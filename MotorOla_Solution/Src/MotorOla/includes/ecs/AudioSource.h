#pragma once
#include "Componente.h"
#if _DEBUG
#include "checkML.h"
#endif
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
	void stop();

private:
	std::string _audioFileName;
	int _channel;
	bool _loop;
	bool _startOnPlay;
};

