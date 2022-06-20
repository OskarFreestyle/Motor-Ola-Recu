#include "ImageComponent.h"
#include "OverlayManager.h"

ImageComponent::~ImageComponent()
{
}

bool ImageComponent::init(const std::map<std::string, std::string>& mapa)
{
	if (mapa.find("positionX") == mapa.end() || mapa.find("positionY") == mapa.end() ||  mapa.find("nombrePanel") == mapa.end() ||
		mapa.find("material") == mapa.end() || mapa.find("dimensionX") == mapa.end() || mapa.find("dimensionY") == mapa.end()) return false;

	std::string s = mapa.at("positionX");
	posX = std::stof(s);
	s = mapa.at("positionY");
	posY = std::stof(s);
    s = mapa.at("nombrePanel");
	nombrePanel = s;
    s = mapa.at("material");
	material = s;
	s = mapa.at("dimensionX");
	dimX = std::stof(s);
	s = mapa.at("dimensionY");
	dimY = std::stof(s);

	OverlayManager::GetInstance()->creaPanel(posX, posY,nombrePanel,material, dimX, dimY);
	

	_inicializado = true;

	return _inicializado;
}
