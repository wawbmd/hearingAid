#include <stdio.h>
#include <math.h>

#include "SineWave.h"

static const ALdouble M_PI = 3.14159265359;

SineWave::SineWave( ALCdevice *openedDevice, ALint sampleRate )
{
    m_buffer = 0;
    m_source = 0;
    m_sampleRate = sampleRate;
    m_frames = 0;
    m_device = openedDevice;
    m_context = 0;
    m_audioData = 0;

	m_context = alcCreateContext(m_device, NULL);
	if (!m_context) {
		printf("Failed to create a context...\n");
		return;
	}
	if (!alcMakeContextCurrent(m_context)) {
		printf("Failed to make context current...\n");
		return;
	}

    alGenBuffers(1, &m_buffer);
    alGenSources(1, &m_source);

}

void SineWave::Cleanup()
{
    alDeleteBuffers( 1, &m_buffer );
    alDeleteSources( 1, &m_source );
}


SineWave::~SineWave()
{
    Cleanup();

    if( m_audioData ) {
        delete []m_audioData;
        m_audioData = 0;
    }
}

void SineWave::CreateWave( Channel outputChannel, ALint startFreq, ALint endFreq, ALint step, ALint seconds )
{
    // Create a series of frames containing tones at the given frequency range.
    // The length and step parameters will determine how long each frequency plays.

    if( outputChannel == SineWave::MONO ) {
        outputChannel = SineWave::STEREO;          // I am not bothering.
    }

    m_outputFormat = outputChannel;
    m_frames = seconds * m_sampleRate * 2;                                      // Total number of frames necessary for buffer ( x2 for stereo output )
    ALint steps_needed = abs( endFreq - startFreq ) / abs( step );              // How many frequency changes are needed between start and end frequency, at the "step" parameter request.
	if (steps_needed == 0)
		steps_needed = 1;
    ALint frames_per_step = m_frames / steps_needed;                            // How many frames are needed for each frequency change to have similar number of frames per frequency
    m_frames = frames_per_step * steps_needed;                                  // How many actual frames will be needed at those numbers.
    
    if( m_audioData ) {
        delete[] m_audioData;
        m_audioData = 0;
    }

    if( endFreq < startFreq && step > 0 ) {                                     // if end frequency is < start, check if step is decreasing as it should
        step *= -1;                                                             //  ... and set it to decrease if necessary.
    }

    m_audioData = new ALushort[m_frames+1];                                     // Create frame buffer
    int curFrame = 0;
    int curStep = 0;
    int curFreq = startFreq;
	int x = 0;
    while( curFrame < m_frames ) {
        if( outputChannel == SineWave::STEREO || outputChannel == SineWave::STEREO_LEFT ) {
            m_audioData[curFrame++] = ( 32767 * sin(( 2 * M_PI * curFreq ) / m_sampleRate * x ));
        } else {
            m_audioData[curFrame++] = 0;
        }

        if( outputChannel == SineWave::STEREO || outputChannel == SineWave::STEREO_RIGHT ) {
            m_audioData[curFrame++] = ( 32767 * sin(( 2 * M_PI * curFreq ) / m_sampleRate * x ));
        } else {
            m_audioData[curFrame++] = 0;
        }

        x ++;                                                                   // Something about sample rate? Dunno. Got this from sample
        curStep ++;                                                             // Index of current frame in this current step
        if( curStep >= frames_per_step ) {                                      // Am I at the end of this current step?
            curFreq += step;                                                    // Yes. Change the current freq for next step
            curStep = 0;                                                        // and reset step counter.
			x = 0;
        }
    }

    // Presto. Audio buffer contains what I want to hear.

}

void SineWave::Play()
{
	if (!alcMakeContextCurrent(m_context)) {
		printf("Failed to make context current...\n");
		return;
	}

    alBufferData( m_buffer, AL_FORMAT_STEREO16, m_audioData, m_frames * 2, m_sampleRate );
    alSourcei( m_source, AL_BUFFER, m_buffer );
    alSourcePlay( m_source );
    int source_state = AL_PLAYING;
    while( source_state == AL_PLAYING ) {                                       // Block until finished playing. This will change in the future.
        alGetSourcei( m_source, AL_SOURCE_STATE, &source_state );
    }
}
