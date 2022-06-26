#pragma once
#ifdef MOTOR_EXPORTS
#define MOTOR_API __declspec(dllexport)
#else
#define MOTOR_API __declspec(dllimport)
#endif
#include "Componente.h"
#if _DEBUG
#include "checkML.h"
#endif
class AudioManager;
using namespace std;

class MOTOR_API AudioSource :	public Componente
{
public:
	AudioSource() {};
	~AudioSource();
	bool init(const std::map<std::string, std::string>& mapa) override;

	void play();
	void pause();
	void stopMusicComponent();

private:
	std::string _audioFileName;
	int _channel;
	bool _loop;
	bool _startOnPlay;
};

