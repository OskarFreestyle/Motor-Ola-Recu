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
	posX = std::stof(s);
	s = mapa.at("positionY");
	posY = std::stof(s);

	s = mapa.at("texto");
	texto = s;
	textoIni = texto;

	s = mapa.at("nombreTexto");
	nombreTexto = s;
	s = mapa.at("tamLetra");
	tamLetra = std::stof(s);
	s = mapa.at("nombrePanel");
	nombrePanel = s;
	
	s = mapa.at("dimensionX");
	dimX = std::stof(s);
	s = mapa.at("dimensionY");
	dimY = std::stof(s);
	s = mapa.at("color");
	color = s;

	OverlayManager::GetInstance()->creaTexto(posX, posY, texto, nombreTexto, tamLetra, nombrePanel, dimX, dimY);
	OverlayManager::GetInstance()->changeTextColor(nombrePanel, nombreTexto, color);

	_inicializado = true;

	return _inicializado;
}

std::string TextComponent::getTexto()
{
	return texto;
}
std::string TextComponent::getTextoIni()
{
	return textoIni;
}
void TextComponent::setTexto(const std::string& s)
{
	texto = s;
	Ogre::TextAreaOverlayElement* t = OverlayManager::GetInstance()->getTexto(nombrePanel,nombreTexto);
	if (t != nullptr)t->setCaption(texto);
}


