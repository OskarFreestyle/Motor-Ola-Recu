#include "AudioManager.h"
#include <iostream>

AudioManager* AudioManager::_singleton = nullptr;

bool AudioManager::Init()
{
	// Si ya existe devuelve false
	if (_singleton != nullptr) return false;

	// Si lo tiene que crear devuelve true
	_singleton = new AudioManager();

	return true;
}

AudioManager::~AudioManager()
{
	result = system->release();
	checkError(result);
}

void AudioManager::update()
{
	result = system->update();
	checkError(result);
}

void AudioManager::loadMusic(int channel, const char* fileName)
{
	result = system->createSound(
		fileName, // path al archivo de sonido
		FMOD_DEFAULT,
		0,
		&sound[channel]);
	checkError(result);
	cont++;
}

void AudioManager::playMusic(int chan, bool loops)
{
	result = system->playSound(sound[chan], 0, false, &channel[chan]);
	checkError(result);

	if (loops)
	{
		result = channel[chan]->setMode(FMOD_LOOP_NORMAL);
		checkError(result);
	}
}

void AudioManager::stopMusic(int chan)
{
	result = channel[chan]->stop();
	checkError(result);
}

void AudioManager::togglePause(int chan)
{
	bool paused;
	result = channel[chan]->getPaused(&paused);
	checkError(result);
	result = channel[chan]->setPaused(!paused);
	checkError(result);
}

void AudioManager::setVolume(int chan, int volume)
{
	result = channel[chan]->setVolume(volume);
	checkError(result);
}

void AudioManager::setPitch(int chan, int pitch)
{
	result = channel[chan]->setPitch(pitch);
	checkError(result);
}

void AudioManager::fadeIn(int chan)
{
	unsigned long long parentclock;
	result = channel[chan]->getDSPClock(NULL, &parentclock);
	result = channel[chan]->addFadePoint(parentclock, 0.0f);
	result = channel[chan]->addFadePoint(parentclock + 500000, 1.0f);
}

void AudioManager::fadeOut(int chan)
{
	unsigned long long parentclock;
	float vol;
	channel[chan]->getVolume(&vol);
	result = channel[chan]->getDSPClock(NULL, &parentclock);
	result = channel[chan]->addFadePoint(parentclock, vol);
	result = channel[chan]->addFadePoint(parentclock + 500000, 0.0f);
}

void AudioManager::checkError(FMOD_RESULT result)
{
	if (result != FMOD_OK) {
		std::cout << FMOD_ErrorString(result) << "\n";
		
	}
}

int AudioManager::getCont()
{
	return cont;
}

bool AudioManager::getMute()
{
	return mute;
}

void AudioManager::setMute(bool m)
{
	mute = m;
	for (int i = 0; i < AudioManager::GetInstance()->getCont(); i++) {
		if (AudioManager::GetInstance()->getMute() == false) {
			if (AudioManager::GetInstance()->getChannel(i) != nullptr)
				AudioManager::GetInstance()->setVolume(i, 1);
		}
		else {
			if (AudioManager::GetInstance()->getChannel(i) != nullptr)
				AudioManager::GetInstance()->setVolume(i, 0);
		}
	}
}

FMOD::Channel* AudioManager::getChannel(int i)
{
	return channel[i];
}



