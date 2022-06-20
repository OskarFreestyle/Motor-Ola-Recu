#pragma once
#ifdef MOTOR_EXPORTS
#define MOTOR_API __declspec(dllexport)
#else
#define MOTOR_API __declspec(dllimport)
#endif 
#include "Componente.h"
#include "PxRigidDynamic.h"

using namespace physx;

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
	PxRigidDynamic* getBody() { return body; };
	PxRigidStatic* getStBody() { return stBody; };
	PxVec3 getVelocity() { return _vel; };
	PxVec3 getAngularVelocity() { return body->getAngularVelocity(); };

	// Setter
	void setVelocity(PxVec3 v);
	void setAngularVelocity(PxVec3 av);

private:
	PxRigidDynamic* body = nullptr;
	PxRigidStatic* stBody = nullptr;

	// Parametros por defecto
	PxVec3 _pos = PxVec3(PxZero);		// position
	PxQuat _ori = PxQuat(PxIdentity);	// orientation
	PxVec3 _vel = PxVec3(PxZero);		// velocity

	bool lockX = true;
};
