#include "AudioSource.h"
#include "AudioManager.h"
#include "LoadResources.h"

AudioSource::~AudioSource()
{
	//stopMusicComponent();
}

bool AudioSource::init(const std::map<std::string, std::string>& mapa) {
	if (mapa.find("audioFileName") == mapa.end() || mapa.find("channel") == mapa.end()
		|| mapa.find("loop") == mapa.end() || mapa.find("startOnPlay") == mapa.end()) return false;

	_audioFileName = mapa.at("audioFileName");

	_channel = std::stoi(mapa.at("channel"));

	std::string s = mapa.at("loop");
	if (s == "true") _loop = true;
	else if (s == "false") _loop = false;
	else return false;

	s = mapa.at("startOnPlay");
	if (s == "true") _startOnPlay = true;
	else if (s == "false") _startOnPlay = false;
	else return false;

	// Se carga siempre
	AudioManager::GetInstance()->loadMusic(_channel, LoadResources::GetInstance()->aud(_audioFileName).c_str());

	// Se reproduce si esta marcada startOnPlay
	if (_startOnPlay&& !AudioManager::GetInstance()->getMute()) play();

	_inicializado = true;

	return _inicializado;
}

void AudioSource::play()
{
	// Comprueba que no haya mute
	if(!AudioManager::GetInstance()->getMute())
		AudioManager::GetInstance()->playMusic(_channel, _loop);
}

void AudioSource::pause()
{
	AudioManager::GetInstance()->togglePause(_channel);
}

void AudioSource::stopMusicComponent()
{
	AudioManager::GetInstance()->stopMusic(_channel);
}