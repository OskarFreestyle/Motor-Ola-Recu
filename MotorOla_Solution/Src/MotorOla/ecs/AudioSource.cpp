#include "AudioSource.h"
#include "AudioManager.h"

AudioSource::AudioSource(int channel,AudioManager* a,const char* s):aud(a),_channel(channel)
{
	aud->loadMusic(_channel, s);
}

AudioSource::~AudioSource()
{

}

void AudioSource::play()
{
	aud->playMusic(_channel, false);
}

void AudioSource::pause()
{
	aud->stopMusic(_channel);
}
