#pragma once
#include "Componente.h"
class ImageComponent :public Componente
{
public:
	ImageComponent() {};
	~ImageComponent();
	bool init(const std::map<std::string, std::string>& mapa) override;
	

private:
	float posX;
	float posY;
	std::string nombrePanel;
	std::string material;
	float dimX;
	float dimY;
	
};

