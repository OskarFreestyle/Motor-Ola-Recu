#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <utility>
#include <limits.h>
#include "Motor.h"

// Ogre
#include <Ogre.h>
#include <OgreEntity.h>
#include <OgreVector3.h>
#include <OgreVector.h>

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
#include "Entidad.h"
#include "Componente.h"
#include "ComponenteRegistro.h"
#include "Transform.h"
#include "Mesh.h"
#include "Light.h"
#include "AudioSource.h"
#include "Collider.h"
#include "Camera.h"
#include "Button.h"
#include "RigidBody.h"

#include <iostream>

Motor::Motor()
{
}

Motor::~Motor()
{
	// Libera la libreria dinamica (el juego)
	FreeLibrary(hDLL);
	// Destruye los managers en orden inverso a la creaci�n (PC: puede que esto no sea necesario porque al cerrar se borran solos)
	if (LuaReader::GetInstance() != nullptr) delete LuaReader::GetInstance();
	if (OverlayManager::GetInstance() != nullptr) delete OverlayManager::GetInstance();
	if (InputManager::GetInstance() != nullptr) delete InputManager::GetInstance();
	if (PhysxManager::GetInstance() != nullptr) delete PhysxManager::GetInstance();
	if (LoadResources::GetInstance() != nullptr) delete LoadResources::GetInstance();
	if (AudioManager::GetInstance() != nullptr) delete AudioManager::GetInstance();
	if (SceneManager::GetInstance() != nullptr) delete SceneManager::GetInstance();
	if (OgreManager::GetInstance() != nullptr) delete OgreManager::GetInstance();
	if (ComponenteFactoria::GetInstance() != nullptr) delete ComponenteFactoria::GetInstance();
#if (defined _DEBUG)
	std::cout << "--------- MOTOR BORRADO CORRECTAMENTE ----------\n";
#endif
}

bool Motor::initMotor()
{
	// Primero inicia los managers
	initManagers();

	// Segundo registra los componentes del motor
	registryComponents();

	// por último intenta cargar la DLL del juego
#if (defined _DEBUG)
	std::cout << "ANTES DE CARGAR JUEGO TRY\n";
#endif

	// El motor intenta cargar un juego, pero si hay algun error se arranca con la funcion loadTestMotorGame
	try {
		loadDLLGame();
	}
	catch (const char* error) {
		std::cout << "Error: " << error << "\n";
		loadTestMotorGame();
	}
	std::cout << "DESPUES DE CARGAR JUEGO TRY\n";

	return true;
}

bool Motor::initManagers()
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
		OverlayManager::Init(OgreManager::GetInstance(), this);
		LuaReader::Init();
	}
	catch (std::exception e) {
#if (defined _DEBUG)
		std::cout << e.what();
#endif
		return false;
	}

#if (defined _DEBUG)
	std::cout << "---------- MANAGERS INICIADOS CORRECTAMENTE ----------\n";
#endif
}

void Motor::registryComponents()
{
	// Apuntar aqui todos los componentes del motor (apuntar solo despues de refactorizar)
	try {
		ComponenteRegistro::ComponenteRegistro<Transform>("transform");
		ComponenteRegistro::ComponenteRegistro<Mesh>("mesh");
		ComponenteRegistro::ComponenteRegistro<Camera>("camera");
		ComponenteRegistro::ComponenteRegistro<Light>("light");
		ComponenteRegistro::ComponenteRegistro<Collider>("collider");
		ComponenteRegistro::ComponenteRegistro<RigidBody>("rigidbody");
		ComponenteRegistro::ComponenteRegistro<Button>("button");
	}
	catch (const char* error) {
		std::cout << "Error registrando los componentes del motor: \n" << error << "\n";
	}

#if (defined _DEBUG)
	std::cout << "---------- COMPONENTES REGISTRADOS ----------\n";
#endif
}

void Motor::mainLoop()
{
#ifdef _DEBUG
	std::cout << "---------- COMIENZA EL BUCLE PRINCIPAL ----------\n";
#endif
	//Actualiza el motor. Bucle input->update/fisicas->render
	stop = false;

	int numFrames = 0;
	int aux = 0;

	while (!stop) {
		// Tiempo cuando se inicia el frame
		frameStart = SDL_GetTicks();

		// Recoge un puntero con el vector de entidades;
		std::vector<Entidad*>* currEntities = SceneManager::GetInstance()->getEntities();

		// Borra el Input del frame anterior
		ih().clearState();
		// Recoge el Input para este frame
		while (SDL_PollEvent(&event))
			ih().update(event);

		// Cierra la aplicacion con ESCAPE
		if (ih().isKeyDown(SDL_SCANCODE_ESCAPE)) {
			stop = true;
			continue;
		}

		// Actualizar las fisicas de las entidades
		pm().runPhysX();

		// Actualiza las entidades (lo cual llama a actualizar cada uno de sus componentes)
		SceneManager::GetInstance()->updateEntidades();

		// TODO creo que quitar en un futuro cuando los overlays sean componentes
		// Actualiza los transforms de las entitys despues de las fisicas
		/*if (OverlayManager::GetInstance() != nullptr) {
			OverlayManager::GetInstance()->update();
		}*/

		// Renderiza un frame
		OgreManager::GetInstance()->update();

		// Se eliminan las entidades marcadas
		SceneManager::GetInstance()->removeEntities();

		// Se cargan nuevas entidades
		SceneManager::GetInstance()->loadEntities();

		// Obtenemos el tiempo del frame
		frameTime = (SDL_GetTicks() - frameStart);

		// Limitamos el frameRate
		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}

		// Actualizamos deltaTime
		deltaTime = (SDL_GetTicks() - frameStart);

		// QUITAR
		std::cout << "GetDeltaTime: " << getDeltaTime() << "\n";
		
		numFrames++;

		if (numFrames >= 60) {
			numFrames -= 60;
			aux++;
			std::cout << "Segundos: " << aux << "\n";
		}
	}
}

void Motor::loadDLLGame()
{
	LPFNDLLFUNC1 lpfnDllFunc1;    // Function pointer
	HRESULT hrReturnVal;
	std::cout<<"Entra en loadDLL\n";
#ifdef NDEBUG
	hDLL = LoadLibrary(L".\\Juego");	// typedef const wchar_t* LPCWSTR, L"..." para indicar que se trata de un long char
#else
	hDLL = LoadLibrary(L".\\Juego_d");	// typedef const wchar_t* LPCWSTR, L"..." para indicar que se trata de un long char
#endif

	if (NULL != hDLL)
	{
		lpfnDllFunc1 = (LPFNDLLFUNC1)GetProcAddress(hDLL, "LoadGame");
		if (NULL != lpfnDllFunc1)
		{
			lpfnDllFunc1(NULL, NULL);
		}
		else throw "Function LoadGame not found in DLL";
	}
	else throw "DLL not found";
}

bool Motor::loadScene(std::string name) {
	try {
		// Borra las entidades de la escena actual
		SceneManager::GetInstance()->pauseScene();
		//Singleton<EntidadManager>::instance()->pauseEntidades();

		// Devuelve la ruta de la escena
		std::string sceneRoute = LoadResources::GetInstance()->scene(name).c_str();

		// Lee la escena cargando todas las entidades y sus componentes
		LuaReader::GetInstance()->readFile(sceneRoute);
	}
	catch (std::exception e) {
#if (defined _DEBUG)
		std::cerr << e.what();
#endif
		return false;
	}
	return true;
}

bool Motor::loadMenu(std::string name,const char*get) {
	try {
		// Borra las entidades de la escena actual
		SceneManager::GetInstance()->pauseScene();
		//Singleton<EntidadManager>::instance()->pauseEntidades();

		// Devuelve la ruta de la escena
		std::string sceneRoute = LoadResources::GetInstance()->scene(name).c_str();

		// Lee la escena cargando todas las entidades y sus componentes
		LuaReader::GetInstance()->readFileMenus(sceneRoute, get);
	}
	catch (std::exception e) {
#if (defined _DEBUG)
		std::cerr << e.what();
#endif
		return false;
	}
	return true;
}

void Motor::loadTestMotorGame() 
{
	loadScene("TestScene.lua");
}

bool Motor::getStop()
{
	return stop;
}

void Motor::setStop(bool s)
{
	stop = s;
}

