#pragma once
#ifdef MOTOR_EXPORTS
#define MOTOR_API __declspec(dllexport)
#else
#define MOTOR_API __declspec(dllimport)
#endif

#include "Componente.h"
#include "PxShape.h"

using namespace physx;
const float BOX_SCALE = 50.0f;

// Forma geométrica para la simulación física (necesita Transform)
class MOTOR_API Collider : public Componente
{
public:
	// OBLIGATORIO EN CADA COMPONENTE
	// Constructor sin parámetros
	Collider();
	// Destructor
	virtual ~Collider();
	// Función para inicializar el componente mediante datos serializados
	bool init(const std::map<std::string, std::string>& mapa);

	// Getter
	PxShape* getShape() { return _shape; };

private:
	PxShape* _shape = nullptr;
	PxGeometry* _geometry = nullptr;
	// forma geometrica por defecto
	PxGeometryType::Enum _type = PxGeometryType::eBOX;
};