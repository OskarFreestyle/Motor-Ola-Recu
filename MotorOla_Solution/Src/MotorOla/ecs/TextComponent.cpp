#include "TextComponent.h"
#include "OverlayManager.h"


TextComponent::~TextComponent()
{
}

bool TextComponent::init(const std::map<std::string, std::string>& mapa)
{
	if (mapa.find("positionX") == mapa.end() || mapa.find("positionY") == mapa.end() || mapa.find("texto") == mapa.end() || mapa.find("nombreTexto") == mapa.end() ||
		mapa.find("tamLetra") == mapa.end() || mapa.find("nombrePanel") == mapa.end() ||  mapa.find("dimensionX") == mapa.end() || mapa.find("dimensionY") == mapa.end()||
		mapa.find("color") == mapa.end()) return false;

	std::string s = mapa.at("positionX");
	_posX = std::stof(s);
	s = mapa.at("positionY");
	_posY = std::stof(s);

	s = mapa.at("texto");
	_texto = s;
	_textoIni = _texto;

	s = mapa.at("nombreTexto");
	_nombreTexto = s;
	s = mapa.at("tamLetra");
	_tamLetra = std::stof(s);
	s = mapa.at("nombrePanel");
	_nombrePanel = s;
	
	s = mapa.at("dimensionX");
	_dimX = std::stof(s);
	s = mapa.at("dimensionY");
	_dimY = std::stof(s);
	s = mapa.at("color");
	_color = s;

	OverlayManager::GetInstance()->creaTexto(_posX, _posY, _texto, _nombreTexto, _tamLetra, _nombrePanel, _dimX, _dimY);
	OverlayManager::GetInstance()->changeTextColor(_nombrePanel, _nombreTexto, _color);

	_inicializado = true;

	return _inicializado;
}

std::string TextComponent::getTexto()
{
	return _texto;
}
std::string TextComponent::getTextoIni()
{
	return _textoIni;
}
void TextComponent::setTexto(const std::string& s)
{
	_texto = s;
	Ogre::TextAreaOverlayElement* t = OverlayManager::GetInstance()->getTexto(_nombrePanel,_nombreTexto);
	if (t != nullptr)t->setCaption(_texto);
}


