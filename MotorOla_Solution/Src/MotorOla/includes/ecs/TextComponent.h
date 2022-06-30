#pragma once
#ifdef MOTOR_EXPORTS
#define MOTOR_API __declspec(dllexport)
#else
#define MOTOR_API __declspec(dllimport)
#endif
#if _DEBUG
#include "checkML.h"
#endif
#include "Componente.h"

// Textos que se renderizan en la escena
class MOTOR_API TextComponent :public Componente
{
public:
	TextComponent() {};
	~TextComponent();
	bool init(const std::map<std::string, std::string>& mapa) override;
	std::string getTexto();
	void setTexto(const std::string& s);
	std::string getTextoIni();


private:
	float _posX;
	float _posY;
	float _dimX;
	float _dimY;
	float _tamLetra;

	std::string _texto;
	std::string _textoIni;
	std::string _nombreTexto;
	std::string _nombrePanel;
	std::string _color; //Solo puede ser Red o Black
};

