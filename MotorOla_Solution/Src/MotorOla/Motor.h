#pragma once

#ifdef MOTOR_EXPORTS
#define MOTOR_API __declspec(dllexport)
#else
#define MOTOR_API __declspec(dllimport)
#endif

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <string>
#include <iostream>
#include <Ogre.h>
#include <Windows.h>

// Declaracion de clases necesarias
class OgreManager;
class InputManager;
class LoadResources;
class FMODAudioManager;
class PhysxManager;
class Entidad;
class EntidadManager;
class Entity;
class OverlayManager;

typedef HRESULT(CALLBACK* LPFNDLLFUNC1)(DWORD, UINT*);

const int FPS = 60;
const int frameDelay = 1000 / FPS;

class MOTOR_API Motor
{
public:
	Motor();
	~Motor();

	/// <summary>
	/// Inicia todos los singletons del motor, registra los componentes y carga la DLL del juego
	/// </summary>
	/// <returns>Devuelve True si todo es correcto</returns>
	bool initMotor();

	/// <summary>
	/// Inicia los singletons del motor
	/// </summary>
	/// <returns></returns>
	bool initManagers();

	/// <summary>
	/// Registra todos los componentes del motor
	/// </summary>
	void registryComponents();

	/// <summary>
	/// Carga Juego.dll y llama a la funcion LoadGame()
	/// </summary>
	void loadDLLGame();

	/// <summary>
	/// Bucle principal de la ejecucion del motor
	/// </summary>
	void mainLoop();

	inline double getDeltaTime() const { return deltaTime / 1000.0f; };

	/// <summary>
	/// Carga una escena del juego
	/// </summary>
	/// <param name="name">name es el archivo "scene.lua"</param>
	bool loadScene(std::string name);
	bool loadMenu(std::string name,const char* get);

	/// <summary>
	/// Funcion para hacer pruebas dentro del motor
	/// </summary>
	void loadTestMotorGame();

	// Getters and Setters
	void setStop(bool s);
	bool getStop();

private:
	// Para recoger los eventos del Input
	SDL_Event event;

	bool stop = false;

	int channel = 0;

	// Contador auxiliar
	Uint32 frameStart;
	Uint32 frameTime;
	Uint32 deltaTime;	// in miliseconds

	HINSTANCE hDLL;               // Handle to DLL
};