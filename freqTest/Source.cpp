#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "AL/al.h"
#include "AL/alc.h"

#include "SineWave.h"

int main()
{
	ALCdevice *device;
	device = alcOpenDevice(NULL);
	if (!device) {
		printf("Failed to open default device...\n");
		exit(1);
	}

	/**
	* Standard hearing test frequencies
	*  125, 250, 500, 1000, 2000, 4000, 8000
	* At volume levels of 0-80 dBHL. If a person cannot hear a given sound at a given volume, 
	*  the results are:
	*	0-10 dBHL:	Normal hearing
	*	20-30 dBHL:	Mild hearing loss
	*	40-60 dBHL:	Moderate loss
	*   70-80 dBHL:	Severe loss
	* Now, I think that's a load of poop, cause according to that, I have moderate loss against all frequencies, and here I thought I was under Mild for at least a few...
	*/
	
	SineWave *sw = new SineWave(device);
	sw->CreateWave(SineWave::STEREO_LEFT, 200, 20000, 100, 10);
	sw->Play();
	delete sw;
	
	alcCloseDevice(device);

	return 0;
}