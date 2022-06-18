#include "MotorOla.h"
#include "Motor.h"

MotorOla::MotorOla()
{
	// Inicia el singleton
	Motor::Init();
}

MotorOla::~MotorOla()
{
	// Destruye el singleton del Motor
	if (Motor::GetInstance() != nullptr) delete Motor::GetInstance();
}

void MotorOla::initMotorOla()
{
	// Pone en marcha los subsistemas
	Motor::GetInstance()->initMotor();
}

void MotorOla::initLoop()
{
	// Arranca el bucle principal
	Motor::GetInstance()->mainLoop();
}
