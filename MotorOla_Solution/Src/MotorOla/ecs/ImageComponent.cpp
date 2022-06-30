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
	_posX = std::stof(s);
	s = mapa.at("positionY");
	_posY = std::stof(s);
    s = mapa.at("nombrePanel");
	_nombrePanel = s;
    s = mapa.at("material");
	_material = s;
	s = mapa.at("dimensionX");
	_dimX = std::stof(s);
	s = mapa.at("dimensionY");
	_dimY = std::stof(s);

	OverlayManager::GetInstance()->creaPanel(_posX, _posY,_nombrePanel,_material, _dimX, _dimY);
	
	_inicializado = true;

	return _inicializado;
}

void ImageComponent::setActive(bool state)
{
	_active = state;
	OverlayManager::GetInstance()->getPanel(_nombrePanel)->setVisible(state);
}
