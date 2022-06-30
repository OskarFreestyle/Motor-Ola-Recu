#pragma once
#include "Componente.h"
#if _DEBUG
#include "checkML.h"
#endif

// Imagenes para el interfaz
class ImageComponent : public Componente
{
public:
	ImageComponent() {};
	~ImageComponent();
	bool init(const std::map<std::string, std::string>& mapa) override;

	void setActive(bool state);
	
private:
	float _posX;
	float _posY;
	float _dimX;
	float _dimY;
	std::string _nombrePanel;
	std::string _material;
};

