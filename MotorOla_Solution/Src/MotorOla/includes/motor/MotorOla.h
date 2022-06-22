#pragma once
#ifdef MOTOR_EXPORTS
#define MOTOR_API __declspec(dllexport)
#else
#define MOTOR_API __declspec(dllimport)
#endif


class MOTOR_API MotorOla
{
public:
	MotorOla();
	// Destructor
	~MotorOla();

	void initMotorOla();

	void initLoop();
};