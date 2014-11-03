#ifndef __sine_wave_h_
#define __sine_wave_h_

#include "AL/al.h"
#include "AL/alc.h"

class SineWave 
{
public:
	SineWave(ALCdevice *openedDevice, ALint sampleRate = 44100);
    ~SineWave();

    typedef enum _tag_thingy_
    {
        MONO,
        STEREO,
        STEREO_LEFT,
        STEREO_RIGHT
    } Channel;

    void CreateWave( Channel outputChannel, ALint startFreq, ALint endFreq, ALint step, ALint seconds );
	void CreateWave(ALint seconds);
    void Play();

	void SaveSineData(const char *filePath);

private:

	SineWave();
	SineWave(SineWave&);
    void Cleanup();

    ALuint m_buffer;
    ALuint m_source;
    Channel m_outputFormat;
    
    ALint m_sampleRate;
    ALushort *m_audioData;
    ALuint m_frames;
    ALCdevice *m_device;
	ALCcontext *m_context;
};


#endif
