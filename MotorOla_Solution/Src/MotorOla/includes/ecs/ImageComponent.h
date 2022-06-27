#pragma once
#include "Componente.h"
#if _DEBUG
#include "checkML.h"
#endif

class ImageComponent : public Componente
{
public:
	ImageComponent() {};
	~ImageComponent();
	bool init(const std::map<std::string, std::string>& mapa) override;

	void setActive(bool state);
	
private:
	float posX;
	float posY;
	std::string nombrePanel;
	std::string material;
	float dimX;
	float dimY;
};

