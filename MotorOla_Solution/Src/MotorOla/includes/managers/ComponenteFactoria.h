#pragma once
#ifdef MOTOR_EXPORTS
#define MOTOR_API __declspec(dllexport)
#else
#define MOTOR_API __declspec(dllimport)
#endif
#if _DEBUG
#include "checkML.h"
#endif
#include <unordered_map>
class Componente;

typedef Componente* (*componentInstanceGenerator) ();

class MOTOR_API ComponenteFactoria {
public:
	~ComponenteFactoria() {};

	/// <summary>
	/// Devuelve una instancia de la clase.
	/// </summary>
	inline static ComponenteFactoria* GetInstance() { return _singleton; }

	/// <summary>
	/// Inicializa la clase SceneManager con los parametros dados si no se ha inicializado antes.
	/// Devuelve true si se inicializa por primera vez y false si ya habia sido inicializada.
	/// </summary>
	static bool Init();

	///<summary>
	///Devuele el componente si esta guardado en mGenerators, si no existe devuelve nullptr
	///</summary>
	Componente* getComponent(std::string name);

	///<summary>
	///Registra el nuevo componente que le pasas, primero el nombre (identificador del ecs) y luego el componente como tal
	///</summary>
	bool registerGenerator(std::string compName, const componentInstanceGenerator& instGenerator);

protected:
	static ComponenteFactoria* _singleton;

	ComponenteFactoria() {};

private:
	// Tabla hash donde guardamos todos los componentes | Nombre -> Componente
	std::unordered_map<std::string, componentInstanceGenerator> _mGenerators;
};