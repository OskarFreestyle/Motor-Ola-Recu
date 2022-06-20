#pragma once
#include "Componente.h"
class AudioManager;
using namespace std;

class AudioSource :	public Componente
{
public:
	AudioSource(int channel, AudioManager*a,const char* s);
	~AudioSource();

	void play();
	void pause();
private:
	AudioManager* aud;
	int _channel;
};

