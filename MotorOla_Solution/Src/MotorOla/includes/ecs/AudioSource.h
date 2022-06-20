#pragma once
#include "Componente.h"
class AudioManager;
using namespace std;

class AudioSource :	public Componente
{
public:
	AudioSource() {};
	~AudioSource();
	bool init(const std::map<std::string, std::string>& mapa) override;

	void play();
	void pause();
private:
	std::string _audioFileName;
	int _channel;
	bool _loop;
};

