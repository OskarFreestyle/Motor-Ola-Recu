#pragma once
#include "ECS.h"
#include <map>
#include <string>
#include "Vectola3D.h"


class Entidad;

class Componente
{
	friend Entidad;

public:
	virtual ~Componente() {};
	virtual bool init(const std::map<std::string, std::string>& mapa) = 0;

	virtual void start() {};
	virtual void update() {};

	// Para triggers
	virtual void onTriggerStart(Entidad* other) {};
	virtual void onTriggerStay(Entidad* other) {};
	virtual void onTriggerEnd(Entidad* other) {};

	// Para las Colisiones
	virtual void onCollisionStart(Entidad* other) {};
	virtual void onCollisionStay(Entidad* other) {};
	virtual void onCollisionEnd(Entidad* other) {};

	virtual void draw() {};

	inline virtual void setActive(bool state) { _active = state; }
	inline void setInitialized(bool state) { _inicializado = state; }
	
	inline bool getActive() const { return _active; }
	inline bool getInitialized() const { return _inicializado; }

	inline void setEntidad(Entidad* entity)	{ _entity = entity;	}
	inline Entidad* getEntidad() const { return _entity; }

protected:
	virtual void render() {};
	bool _active = true;
	bool _inicializado= false;
	Entidad* _entity = nullptr;
};


