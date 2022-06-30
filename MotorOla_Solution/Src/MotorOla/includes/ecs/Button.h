#pragma once
#include "Componente.h"
#ifdef MOTOR_EXPORTS
#define MOTOR_API __declspec(dllexport)
#else
#define MOTOR_API __declspec(dllimport)
#endif

#if _DEBUG
#include "checkML.h"
#endif

class Motor;

// Utiliza una imagen y detecta cuando se clicka en ella
class MOTOR_API Button :public Componente
{
public:
	enum Type {
		CHANGE_SCENE=0,
		VOLUME=1,
		EXIT=2
	};

	Button() {};
	~Button();
	bool init(const std::map<std::string, std::string>& mapa) override;
	virtual void update();
	
protected:
	Type _type;

	float _posX; 
	float _posY; 
	float _tamLetra;
	float _dimX;
	float _dimY;
	
	std::string _texto; 
	std::string _nombrePanel;  
	std::string _nombreTexto; 
	std::string _material; 
	std::string _nextScene = "";

	/// <summary>
	/// Variables para delay entre que se pulsa y se hace la función onClick
	/// </summary>
	bool _isClick = false;
	clock_t _timeClick = 0;
	clock_t _clickDelay;

	/// <summary>
	/// Comprueba si se pulsa el botón
	/// </summary>
	/// <returns>True si se ha pulsado</returns>
	bool isClicked();

	/// <summary>
	/// Funcionalidades 
	/// </summary>
	virtual void onClick();
};

