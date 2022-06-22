#pragma once

#ifdef MOTOR_EXPORTS
#define MOTOR_API __declspec(dllexport)
#else
#define MOTOR_API __declspec(dllimport)
#endif
#if _DEBUG
#include "checkML.h"
#endif
#include <map>
#include <string>
#include <filesystem>

using namespace std::filesystem;
using namespace std;


//RUTA PARA BUSCAR LOS ASSETS
//static const std::string ASSETS = "./Assets";	// Codigo unico para la entrega, perdonanos Pedro P.
static const std::string ASSETS = "../../Exes/Assets/";


class MOTOR_API LoadResources {
public:
	~LoadResources();

	/// <summary>
	/// Devuelve una instancia de la clase.
	/// </summary>
	inline static LoadResources* GetInstance() { return _singleton; }

	/// <summary>
	/// Inicializa la clase LoadResources con los parametros dados si no se ha inicializado antes.
	/// Devuelve true si se inicializa por primera vez y false si ya habia sido inicializada.
	/// </summary>
	static bool Init();

	string mes(string m);
	string aud(string name);
	string tex(string tex);
	string scene(string scene);
	string prefab(string prefab);

protected:
	static LoadResources* _singleton;

	LoadResources();	// Constructor vacío

private:
	map<string, string>mesh;//.mesh
	map<string, string>audio;//.mp3,.ogg,.wav
	map<string, string>textures;//.png,.jpg,.bmp
	map<string, string>scenes;//.lua
	map<string, string>prefabs;
	//Si hay otro tipo añadir el map
	void search(path p);
	void load(path p, size_t end, size_t pathLenght);
};

