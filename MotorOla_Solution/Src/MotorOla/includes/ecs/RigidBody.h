#pragma once
#ifdef MOTOR_EXPORTS
#define MOTOR_API __declspec(dllexport)
#else
#define MOTOR_API __declspec(dllimport)
#endif 

#include "Componente.h"
#include "PxRigidDynamic.h"

using namespace physx;

// Define el comportamiento fisico de un objeto (Necesita Collider)
class MOTOR_API RigidBody : public Componente
{
public:
	// OBLIGATORIO EN CADA COMPONENTE
	// Constructor sin parámetros 
	RigidBody();
	// Destructor
	virtual ~RigidBody();
	// Función para inicializar el componente mediante datos serializados
	bool init(const std::map<std::string, std::string>& mapa);

	// Getter
	PxRigidDynamic* getBody() { return _body; };
	PxRigidStatic* getStBody() { return _stBody; };
	PxVec3 getVelocity() { return _vel; };
	PxVec3 getAngularVelocity() { return _body->getAngularVelocity(); };

	// Setter
	void setVelocity(PxVec3 v);
	void setAngularVelocity(PxVec3 av);

private:
	PxRigidDynamic* _body = nullptr;
	PxRigidStatic* _stBody = nullptr;

	// Parametros por defecto
	PxVec3 _pos = PxVec3(PxZero);		// position
	PxQuat _ori = PxQuat(PxIdentity);	// orientation
	PxVec3 _vel = PxVec3(PxZero);		// velocity

	// Constrains
	bool _lockPosX = false;
	bool _lockPosY = false;
	bool _lockPosZ = false;
	bool _lockRotX = false;
	bool _lockRotY = false;
	bool _lockRotZ = false;
};
