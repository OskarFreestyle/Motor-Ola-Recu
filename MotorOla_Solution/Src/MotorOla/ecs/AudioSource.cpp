#include "AudioSource.h"
#include "AudioManager.h"
#include "LoadResources.h"

AudioSource::~AudioSource()
{
}

bool AudioSource::init(const std::map<std::string, std::string>& mapa) {
	if (mapa.find("audioFileName") == mapa.end() || mapa.find("channel") == mapa.end() || mapa.find("loop") == mapa.end()) return false;

	_audioFileName = mapa.at("audioFileName");

	_channel = std::stoi(mapa.at("channel"));

	std::string s = mapa.at("loop");
	if (s == "true") _loop = true;
	else if (s == "false") _loop = false;
	else return false;

	AudioManager::GetInstance()->loadMusic(_channel, LoadResources::GetInstance()->aud(_audioFileName).c_str());

	_inicializado = true;

	return _inicializado;
}

void AudioSource::play()
{
	AudioManager::GetInstance()->playMusic(_channel, _loop);
}

void AudioSource::pause()
{
	AudioManager::GetInstance()->stopMusic(_channel);
}
