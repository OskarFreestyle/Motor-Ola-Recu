#pragma once
#ifdef MOTOR_EXPORTS
#define MOTOR_API __declspec(dllexport)
#else
#define MOTOR_API __declspec(dllimport)
#endif

// Clase auxiliar para manejar el singleton Motor
class MOTOR_API MotorOla
{
public:
	MotorOla();
	~MotorOla();

	/// <summary>
	/// Pone en marcha los subsistemas del motor
	/// </summary>
	void initMotorOla();

	/// <summary>
	/// Arranca el bucle principal del motor
	/// </summary>
	void initLoop();
};