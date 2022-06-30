#include "RigidBody.h"
#include "PhysxManager.h"
#include "Collider.h"
#include "Transform.h"

#if _DEBUG
	const bool debugCom = false;
#endif

RigidBody::RigidBody()
{
	
}

RigidBody::~RigidBody()
{
	if (_body) {
		_body->release();
		_body = nullptr;
	}
	if (_stBody) {
		_stBody->release();	// Also delete the shape
		_stBody = nullptr;
	}
}

bool RigidBody::init(const std::map<std::string, std::string>& mapa)
{
	// Comprueba que exista el componente transform y este inicializado
	if (!_entity->hasComponent<Transform>() || !_entity->getComponent<Transform>()->getInitialized()) return false;

	// Comprueba que exista el componente collider y este inicializado
	if (!_entity->hasComponent<Collider>() || !_entity->getComponent<Collider>()->getInitialized()) return false;

	Transform* tr = getEntidad()->getComponent<Transform>();

	// Valores para guardar los datos leidos
	float posX=0.f, posY=0.f, posZ=0.f;
	float oriX=0.f, oriY=0.f, oriZ=0.f, oriW=1.f;
	float velX=0.f, velY=0.f, velZ=0.f;

	// Valores auxiliares para guardar el tamano de cada string
	std::string::size_type sact = 0, stot = 0;
	std::string tempString; // string auxiliar para guardar la subcadena actual

	// Si no existe / no tiene el componente Transform, lee los datos de LUA
	if (!tr || tr->getLocalPosition() == Vectola3D()) {
		// comprobar que la secci�n existe
		if (mapa.find("position") != mapa.end())
		{
			// Lee los datos de la posicion
			std::string posString = mapa.at("position");
			posX = stof(posString, &sact); stot = sact + 1; sact = 0;
			tempString = posString.substr(stot);
			posY = stof(tempString, &sact); stot += sact + 1; sact = 0;
			tempString = posString.substr(stot);
			posZ = stof(tempString, &sact); stot += sact + 1; sact = 0;

			// Establece las propiedades leidas...
			_pos = PxVec3(posX, posY, posZ);
		}

		// comprobar que la secci�n existe
		if (mapa.find("orientation") != mapa.end())
		{
			// Lee los datos de la orientaci�n
			std::string oriString = mapa.at("orientation");
			oriX = stof(oriString, &sact); stot = sact + 1; sact = 0;
			tempString = oriString.substr(stot);
			oriY = stof(tempString, &sact); stot += sact + 1; sact = 0;
			tempString = oriString.substr(stot);
			oriZ = stof(tempString, &sact); stot += sact + 1; sact = 0;
			tempString = oriString.substr(stot);
			oriW = stof(tempString, &sact); stot += sact + 1; sact = 0;

			// Establece las propiedades leidas...
			_ori = PxQuat(oriX, oriY, oriZ, oriW);
		}
	}
	else {
		Vectola3D pos = tr->getLocalPosition();
		Quaterniola rot = tr->getLocalRotation();
		_pos = PxVec3(pos.getX(), pos.getY(), pos.getZ());
		_ori = PxQuat(rot.v.getX(), rot.v.getX(), rot.v.getX(), rot.s);
	}

	// comprobar que la secci�n existe
	if (mapa.find("velocity") != mapa.end())
	{
		// Lee los datos de la velocidad
		std::string velString = mapa.at("velocity");
		velX = stof(velString, &sact); stot = sact + 1; sact = 0;
		tempString = velString.substr(stot);
		velY = stof(tempString, &sact); stot += sact + 1; sact = 0;
		tempString = velString.substr(stot);
		velZ = stof(tempString, &sact); stot += sact + 1; sact = 0;

		// Establece las propiedades leidas...
		_vel = PxVec3(velX, velY, velZ);
		
	}	

	// Position and orientation in world space (Physx pose)
	PxTransform pose = PxTransform(_pos, _ori);

	// Establece si es estatico o dinamico
	bool estatico = false;
	if (mapa.find("static") != mapa.end())
	{
		// Establece el tipo de simulacion fisica
		std::string staticString = mapa.at("static");
		if (staticString == "true")
			estatico = true;
	}

	// Crea el RigidBody con todos los parametros recogidos
	PxShape* shape = getEntidad()->getComponent<Collider>()->getShape();
	if (shape) {
		if (estatico)
			_stBody = pm().createStaticRigid(pose, shape);
		else
			_body = pm().createDynamic(pose, shape, _vel);
#if _DEBUG
		if (debugCom) std::cout << "Shape first - Rigidbody: set collider!\n";
#endif
	}
	else {
		if (estatico)
			_stBody = pm().createStaticRigid(pose);
		else
			_body = pm().createDynamic(pose, _vel);
#if _DEBUG
		if (debugCom) std::cout << "Rigidbody - sin collider...\n";
#endif
	}

	// Establece la densidad del objeto
	float density = 1000.f;
	if (mapa.find("density") != mapa.end())
	{
		// Establece la masa y la inercia
		std::string densityString = mapa.at("density");
		density = stof(densityString);
		if (_body) PxRigidBodyExt::updateMassAndInertia(*_body, density);
		else {
#if _DEBUG
			if (debugCom) std::cout << "RigidBody - ERROR: un static no usa density!\n";
#endif
			return false; // solo para dynamic!
		}
	}

	// Establece si el objeto mantiene la propiedad de ser afectado por la gravedad
	bool gravity = true;
	if (mapa.find("gravity") != mapa.end())
	{
		// Establece la gravedad en el objeto
		std::string gravityString = mapa.at("gravity");
		if (gravityString == "false")
		{
			gravity = false;
			_body->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
		}
	}
	if (gravity && _body) _body->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);

	// A�ade la entidad asociado al manager de phyx
	int id_ = getEntidad()->getID();
	pm().addEntityID(id_);

	if (mapa.find("lockPosX") != mapa.end())
	{
		tempString = mapa.at("lockPosX");
		if (tempString == "true") _lockPosX = true;
		else if (tempString == "false") _lockPosX = false;
		else return false;
		_body->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_X, _lockPosX);
	}
	if (mapa.find("lockRotX") != mapa.end()) {
		tempString = mapa.at("lockRotX");
		if (tempString == "true") _lockRotX = true;
		else if (tempString == "false") _lockRotX = false;
		else return false;
		_body->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, _lockRotX);
	}
	if (mapa.find("lockPosY") != mapa.end())
	{
		tempString = mapa.at("lockPosY");
		if (tempString == "true") _lockPosY = true;
		else if (tempString == "false") _lockPosY = false;
		else return false;
		_body->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, _lockPosY);
	}
	if (mapa.find("lockRotY") != mapa.end()) {
		tempString = mapa.at("lockRotY");
		if (tempString == "true") _lockRotY = true;
		else if (tempString == "false") _lockRotY = false;
		else return false;
		_body->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, _lockRotY);
	}	
	if (mapa.find("lockPosZ") != mapa.end())
	{
		tempString = mapa.at("lockPosZ");
		if (tempString == "true") _lockPosZ = true;
		else if (tempString == "false") _lockPosZ = false;
		else return false;
		_body->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, _lockPosZ);
	}
	if (mapa.find("lockRotZ") != mapa.end()) {
		tempString = mapa.at("lockRotZ");
		if (tempString == "true") _lockRotZ = true;
		else if (tempString == "false") _lockRotZ = false;
		else return false;
		_body->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, _lockRotZ);
	}

	return true;
}

void RigidBody::setVelocity(PxVec3 v)
{
	_vel = v;
	_body->setLinearVelocity(v);
}

void RigidBody::setAngularVelocity(PxVec3 av)
{
	_body->setAngularVelocity(av);
}
