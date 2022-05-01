#pragma once

#ifdef MOTOR_EXPORTS
#define MOTOR_API __declspec(dllexport)
#else
#define MOTOR_API __declspec(dllimport)
#endif

#include <vector>
#include <memory>
#include <string>
#include <stack>

#include "utils\Singleton.h"
class Entidad;


MOTOR_API class EntidadManager : public Singleton<EntidadManager> {
	friend Singleton<EntidadManager>;

	using uptr_ent = std::unique_ptr<Entidad>;
public:
	EntidadManager() {};
	virtual ~EntidadManager() {};

	MOTOR_API Entidad* addEntidad();
	MOTOR_API Entidad* addEntidad(std::string entityName, int id);
	MOTOR_API void update();
	MOTOR_API void draw();
	MOTOR_API void refresh();
	MOTOR_API void pauseEntidades();
	MOTOR_API void reanudeEntidades();
	MOTOR_API void sincronizaVectorEnt();
	
	MOTOR_API Entidad* getEntidadByID(int id);

private:
	std::vector<uptr_ent> entitys_;
	//std::vector<uptr_ent> entitiesToLoad_;		// Vector con las entidades a cargar al final del frame

	// Idea para guardar las entidades al poner nuevas escenas
	//std::stack <std::vector<uptr_ent>> pilaEntidades_;
};
