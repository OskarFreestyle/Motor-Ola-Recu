#pragma once
#include "Componente.h"
class TextComponent :public Componente
{
public:
	TextComponent() {};
	~TextComponent();
	bool init(const std::map<std::string, std::string>& mapa) override;
	std::string getTexto();
	void setTexto(const std::string& s);

private:
	float posX;
	float posY;
	std::string texto;
	std::string nombreTexto;
	float tamLetra;
	std::string nombrePanel;
	float dimX;
	float dimY;
	std::string color;//Solo puede ser Red o Black
};

