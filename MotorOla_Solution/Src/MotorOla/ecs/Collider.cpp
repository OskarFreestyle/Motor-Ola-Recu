#include "Collider.h"
#include "PhysxManager.h"
#include "RigidBody.h"
#include "Transform.h"


#if _DEBUG
	const bool debugCom = false;
#endif

Collider::Collider()
{
	
}

Collider::~Collider()
{
	if (_geometry) {
		delete _geometry;
		_geometry = nullptr;
	}
	if (_shape) {
		_shape->release();
		_shape = nullptr;
	}

}

bool Collider::init(const std::map<std::string, std::string>& mapa)
{
	// Comprueba que exista el componente transform y este inicializado
	if (!_entity->hasComponent<Transform>() || !_entity->getComponent<Transform>()->getInitialized()) return false;

	// comprobar que la secci�n existe
	if (mapa.find("type") == mapa.end())
		return false;

	// variables principales
	
	PxMaterial* material = pm().getMaterial();	// Establece el tipo de material

	// identifica el tipo de geometr�a
	std::string typeString = mapa.at("type");

	if (typeString == "sphere") {
		// comprobar que la secci�n existe
		if (mapa.find("radius") == mapa.end())
			return false;

		// establecemos config. de geometr�a
		_type = PxGeometryType::eSPHERE;

		// traducci�n
		std::string radiusString = mapa.at("radius");
		float rad = stof(radiusString);
		//
		if (rad < 0)
			return false;

		// Crea la forma del collider (esfera)
		_geometry = new PxSphereGeometry(rad * _entity->getComponent<Transform>()->getScale().getX() * 100); /// �la escala es igual respecto a Ogre?
	}
	else if (typeString == "box") {
		// comprobar que la secci�n existe
		if (mapa.find("x") == mapa.end() ||
			mapa.find("y") == mapa.end() ||
			mapa.find("z") == mapa.end())
				return false;

		// establecemos config. de geometr�a
		_type = PxGeometryType::eBOX;

		// traducci�n
		std::string xString = mapa.at("x");
		float dimX = stof(xString);
		std::string yString = mapa.at("y");
		float dimY = stof(yString);
		std::string zString = mapa.at("z");
		float dimZ = stof(zString);
		
		if (dimX < 0 || dimY < 0 || dimZ < 0)
			return false;

		// Crea la forma del collider (cubo)
		_geometry = new PxBoxGeometry(PxVec3(dimX * _entity->getComponent<Transform>()->getScale().getX(),
			dimY * _entity->getComponent<Transform>()->getScale().getY(), 
			dimZ * _entity->getComponent<Transform>()->getScale().getZ()) * BOX_SCALE); /// �escala?
	}
	

	// Localiza el parametro que indica si el collider es tipo 'trigger'
	if (mapa.find("trigger") != mapa.end()) 
	{
		// Establece el tipo de simulacion fisica
		std::string triggerString = mapa.at("trigger");
		if (triggerString == "true")
			_shape = pm().createTriggerShape(*_geometry, *material, false);
	}
	// Si no se ha definido dicho parametro o es 'false'
	if (_shape == nullptr) {
		_shape = pm().createShape(*_geometry, *material, false); // NO TRIGGER, NO POO
	}

	// Recoge si existe el componente RigidBody
	RigidBody* body = getEntidad()->getComponent<RigidBody>();
	if (body) {
		if (body->getBody() && body->getBody()->getNbShapes() == 0)				// dynamic
			body->getBody()->attachShape(*_shape);
		else if (body->getStBody() && body->getStBody()->getNbShapes() == 0)	// static
			body->getStBody()->attachShape(*_shape);
#if _DEBUG
		if (debugCom) std::cout << "RB first - Collider: shape attach!\n";
#endif
	}
#if _DEBUG
	else
		if (debugCom) std::cout << "Collider: no body, no shape attach...\n";
#endif
	
	_inicializado = true;

	return true;
}