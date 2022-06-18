#pragma once
#include "Componente.h"
class Motor;
using CallBackOnClick = void(Motor* m);


class Button :public Componente
{
public:
	enum Type {
		CHANGE_SCENE=0,
		VOLUME=1,
		EXIT=2
	};
	Button() {};
	~Button() {};
	bool init(const std::map<std::string, std::string>& mapa) override;
	
private:
	float x; 
	float y; 
	std::string texto; 
	std::string nombrePanel;  
	std::string nombreTexto; 
	float tamLetra;
	std::string material; 
	float dimX;
	float dimY;
	Type t;
	std::string nextScene = "";
	static void changeScene(Motor* m);
	static void volume(Motor* m);
	static void exit(Motor* m);
	

};

