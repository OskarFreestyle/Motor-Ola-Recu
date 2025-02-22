#pragma once
#ifdef MOTOR_EXPORTS
#define MOTOR_API __declspec(dllexport)
#else
#define MOTOR_API __declspec(dllimport)
#endif
#if _DEBUG
#include "checkML.h"
#endif
#include <fmod.hpp>
#include <fmod_errors.h>

const int NUM_CHANNELS = 24;

// Manejador de audio con 24 canales
class MOTOR_API AudioManager {
public:
	~AudioManager();

	/// <summary>
	/// Devuelve una instancia de la clase.
	/// </summary>
	inline static AudioManager* GetInstance() { return _singleton; }

	/// <summary>
	/// Inicializa la clase AudioManager con los parametros dados si no se ha inicializado antes.
	/// Devuelve true si se inicializa por primera vez y false si ya habia sido inicializada.
	/// </summary>
	static bool Init();

	void update();

	void loadMusic(int channel, const char* fileName);
	void playMusic(int channel, bool loops);
	void stopMusic(int channel);
	void togglePause(int channel);
	void setVolume(int chan, int volume);
	void setPitch(int chan, int pitch);
	void fadeIn(int chan);
	void fadeOut(int chan);
	void checkError(FMOD_RESULT result);
	int getCont();
	bool getMute();
	void setMute(bool m);
	void stopAllChannels();
	FMOD::Channel* getChannel(int i);

protected:
	static AudioManager* _singleton;

	AudioManager() {
		for (int i = 0; i < NUM_CHANNELS; i++) channelUsed[i] = false;
		result = System_Create(&system);
		checkError(result);
		result = system->init(128, FMOD_INIT_NORMAL, 0);
		checkError(result);
	};

private:
	FMOD::System* system = nullptr;
	FMOD_RESULT result = FMOD_OK;
	FMOD::Channel* channel[NUM_CHANNELS];
	FMOD::Sound* sound[NUM_CHANNELS];
	bool channelUsed[NUM_CHANNELS];
	bool mute = false;
	int cont = 0;
};