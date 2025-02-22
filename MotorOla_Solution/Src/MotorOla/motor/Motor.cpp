#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <utility>
#include <limits.h>
#include "Motor.h"

// Managers
#include "OgreManager.h"
#include "InputManager.h"
#include "LoadResources.h"
#include "AudioManager.h"
#include "OverlayManager.h"
#include "PhysxManager.h"
#include "SceneManager.h"
#include "LuaReader.h"
#include "ComponenteFactoria.h"

// Entidades y Componentes
#include "ComponenteRegistro.h"
#include "Transform.h"
#include "Mesh.h"
#include "Light.h"
#include "AudioSource.h"
#include "Collider.h"
#include "Camera.h"
#include "Button.h"
#include "TextComponent.h"
#include "ImageComponent.h"
#include "RigidBody.h"
#include <time.h>


Motor* Motor::_singleton = nullptr;

bool Motor::Init()
{
	// Si ya existe devuelve false
	if (_singleton != nullptr) return false;

	// Si lo tiene que crear devuelve true
	_singleton = new Motor();
	return true;
}

Motor::Motor()
{
}

Motor::~Motor()
{
	// Destruye los managers en orden inverso a la creaci�n (PC: puede que esto no sea necesario porque al cerrar se borran solos)
	if (LuaReader::GetInstance() != nullptr) delete LuaReader::GetInstance();
	if (OverlayManager::GetInstance() != nullptr) delete OverlayManager::GetInstance();
	if (InputManager::GetInstance() != nullptr) delete InputManager::GetInstance();
	if (LoadResources::GetInstance() != nullptr) delete LoadResources::GetInstance();
	if (AudioManager::GetInstance() != nullptr) delete AudioManager::GetInstance();
	if (SceneManager::GetInstance() != nullptr) delete SceneManager::GetInstance();
	if (PhysxManager::GetInstance() != nullptr) delete PhysxManager::GetInstance();
	if (OgreManager::GetInstance() != nullptr) delete OgreManager::GetInstance();
	if (ComponenteFactoria::GetInstance() != nullptr) delete ComponenteFactoria::GetInstance();

	// Libera la libreria dinamica (el juego)
	if (hDLL != NULL)
	{
		LPFNDLLFUNC1 lpfnDllFunc1;    // Function pointer
		lpfnDllFunc1 = (LPFNDLLFUNC1)GetProcAddress(hDLL, "deleteGame");
		if (NULL != lpfnDllFunc1)
		{
			lpfnDllFunc1(NULL, NULL);
		}
		else throw "Function LoadGame not found in DLL";

		FreeLibrary(hDLL);
	}
	
	
#if (defined _DEBUG)
	std::cout << "--------- MOTOR BORRADO CORRECTAMENTE ----------\n";
#endif
}

void Motor::initMotor()
{
	//Inicia la semilla de aleatoriedad
	srand(time(NULL));

	// Primero inicia los managers
	initManagers();

	// Segundo registra los componentes del motor
	registryComponents();

	// El motor intenta cargar un juego, pero si hay algun error se arranca con la funcion loadTestMotorGame
	loadDLLGame();

}

void Motor::initManagers()
{
	// Intenta iniciar todos los singletons del motor
	try {
		// Ya cambiados
		ComponenteFactoria::Init();
		SceneManager::Init();
		AudioManager::Init();
		LoadResources::Init();
		PhysxManager::Init();
		InputManager::Init();
		OgreManager::Init();
		OverlayManager::Init(OgreManager::GetInstance());
		LuaReader::Init();
	}
	catch (std::exception& error) {
		std::cout << "Error: " << error.what() << "\n";
		throw std::exception("Error iniciando los managers\n");
	}

#if (defined _DEBUG)
	std::cout << "---------- MANAGERS INICIADOS CORRECTAMENTE ----------\n";
#endif
}

void Motor::registryComponents()
{
	// Apuntar aqui todos los componentes del motor (apuntar solo despues de refactorizar)
	try {
		ComponenteRegistro::ComponenteRegistro<AudioSource>("audioSource");
		ComponenteRegistro::ComponenteRegistro<Button>("button");
		ComponenteRegistro::ComponenteRegistro<Camera>("camera");
		ComponenteRegistro::ComponenteRegistro<Collider>("collider");
		ComponenteRegistro::ComponenteRegistro<ImageComponent>("image");
		ComponenteRegistro::ComponenteRegistro<Light>("light");
		ComponenteRegistro::ComponenteRegistro<Mesh>("mesh");
		ComponenteRegistro::ComponenteRegistro<RigidBody>("rigidbody");
		ComponenteRegistro::ComponenteRegistro<TextComponent>("texto");
		ComponenteRegistro::ComponenteRegistro<Transform>("transform");
	}
	catch (...) {
		throw std::exception("Error registrando los componentes del motor\n");
	}

#if (defined _DEBUG)
	std::cout << "---------- COMPONENTES REGISTRADOS ----------\n";
#endif
}

void Motor::loadDLLGame()
{
	LPFNDLLFUNC1 lpfnDllFunc1;    // Function pointer
	HRESULT hrReturnVal;

#if (defined _DEBUG)
	hDLL = LoadLibrary(L".\\Juego_d");	// typedef const wchar_t* LPCWSTR, L"..." para indicar que se trata de un long char
#else
	hDLL = LoadLibrary(L".\\Juego");	// typedef const wchar_t* LPCWSTR, L"..." para indicar que se trata de un long char
#endif

	if (NULL != hDLL)
	{
		lpfnDllFunc1 = (LPFNDLLFUNC1)GetProcAddress(hDLL, "LoadGame");
		if (NULL != lpfnDllFunc1)
		{
			lpfnDllFunc1(NULL, NULL);
		}
		else throw std::exception("Function LoadGame not found in DLL");
	}
	else throw std::exception("DLL not found");
}

void Motor::mainLoop()
{
#ifdef _DEBUG
	std::cout << "---------- COMIENZA EL BUCLE PRINCIPAL ----------\n";
#endif

	//Actualiza el motor. Bucle input->update/fisicas->render
	stop = false;

	while (!stop) {
		// Tiempo cuando se inicia el frame
		frameStart = SDL_GetTicks();
		// Borra el Input del frame anterior
		ih().clearState();
		// Recoge el Input para este frame
		while (SDL_PollEvent(&event)) ih().update(event);
		// Actualizar las fisicas de las entidades
		pm().runPhysX();
		// Actualiza las entidades (lo cual llama a actualizar cada uno de sus componentes)
		SceneManager::GetInstance()->updateEntidades();
		// Renderiza un frame
		OgreManager::GetInstance()->update();
		//Actualiza el sonido
		AudioManager::GetInstance()->update();
		// Se eliminan las entidades marcadas
		SceneManager::GetInstance()->removeEntities();
		// Se cargan nuevas entidades
		SceneManager::GetInstance()->loadEntities();
		// Obtenemos el tiempo del frame
		frameTime = (SDL_GetTicks() - frameStart);
		// Limitamos el frameRate
		if (frameDelay > frameTime) SDL_Delay(frameDelay - frameTime);
		// Actualizamos deltaTime
		deltaTime = (SDL_GetTicks() - frameStart);
	}
}

bool Motor::loadScene(std::string name) {
	try {
		// Borra las entidades de la escena actual
		SceneManager::GetInstance()->pauseScene();

		// Devuelve la ruta de la escena
		std::string sceneRoute = LoadResources::GetInstance()->scene(name).c_str();

		// Lee la escena cargando todas las entidades y sus componentes
		LuaReader::GetInstance()->readScene(sceneRoute);
	}
	catch (std::exception e) {
#if (defined _DEBUG)
		std::cerr << e.what();
#endif
		return false;
	}
	return true;
}


