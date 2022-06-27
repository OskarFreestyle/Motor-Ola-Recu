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
	float posX; 
	float posY; 
	std::string texto; 
	std::string nombrePanel;  
	std::string nombreTexto; 
	float tamLetra;
	std::string material; 
	float dimX;
	float dimY;
	Type type;
	std::string nextScene = "";
	bool isClick = false;
	clock_t inClick = 0;
	clock_t clickDelay;

	/// <summary>
	/// Comprueba si se pulsa el botón
	/// </summary>
	/// <returns></returns>
	bool isClicked();

	/// <summary>
	/// Funcionalidades 
	/// </summary>
	virtual void onClick();
};

