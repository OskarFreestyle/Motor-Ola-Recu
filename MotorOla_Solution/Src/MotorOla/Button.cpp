#include "Button.h"
#include "OverlayManager.h"
#include "SceneManager.h"
#include "AudioManager.h"
#include "Motor.h"
#include "InputManager.h"


//std::string nS = " ";
Button::~Button() {
	OverlayManager::GetInstance()->clear();
}

bool Button::init(const std::map<std::string, std::string>& mapa) {
	if (mapa.find("positionX") == mapa.end() || mapa.find("positionY") == mapa.end() || mapa.find("texto") == mapa.end() || mapa.find("nombrePanel") == mapa.end() ||
		mapa.find("nombreTexto") == mapa.end() || mapa.find("tamLetra") == mapa.end() || mapa.find("material") == mapa.end()||mapa.find("dimensionX")==mapa.end()||mapa.find("dimensionY")==mapa.end(),
		mapa.find("type")==mapa.end(),mapa.find("nextScene")==mapa.end()) return false;

	

	std::string s = mapa.at("positionX");
	posX = std::stof(s);
	s = mapa.at("positionY");
	posY = std::stof(s);

	s = mapa.at("texto");
	texto = s;

	s = mapa.at("nombrePanel");
	nombrePanel = s;
	s = mapa.at("nombreTexto");
	nombreTexto = s;
	s = mapa.at("tamLetra");
	tamLetra = std::stof(s);
	s = mapa.at("material");
	material = s;
	s = mapa.at("dimensionX");
	dimX = std::stof(s);
	s = mapa.at("dimensionY");
	dimY = std::stof(s);
	
	OverlayManager::GetInstance()->creaBoton(posX, posY, texto, nombrePanel, nombreTexto, tamLetra, material, dimX, dimY);
	s = mapa.at("type");
	if (s == "CHANGE_SCENE") {
		type = Type::CHANGE_SCENE;
		s = mapa.at("nextScene");
		nextScene = s;
		//nS = nextScene;
		//OverlayManager::GetInstance()->setCallBackToButton(nombrePanel,changeScene);
		
	}
	else if (s == "VOLUME") {
		type = Type::VOLUME;
		//OverlayManager::GetInstance()->setCallBackToButton(nombrePanel, volume);
	}
	else if (s == "EXIT") {
		type = Type::EXIT;
		//OverlayManager::GetInstance()->setCallBackToButton(nombrePanel,exit);

	}
	std::cout << "Tipo: " << type << "\n";
	std::cout << nextScene << "\n";
	_inicializado = true;

	return true;
}

void Button::update()
{
	if (isClicked()) {
		onClick();
	}

}


bool Button::isClicked() {
	return (ih().getMouseButtonState(ih().LEFT)) &&
		(ih().getMousePos().first > posX * OgreManager::GetInstance()->getWindowWidth() && ih().getMousePos().first < (posX + dimX)* OgreManager::GetInstance()->getWindowWidth() &&
			ih().getMousePos().second>posY * OgreManager::GetInstance()->getWindowHeight() && ih().getMousePos().second < (posY + dimY)* OgreManager::GetInstance()->getWindowHeight());

}

void Button::onClick()
{
	switch (type) {
	case Type::CHANGE_SCENE:
		SceneManager::GetInstance()->newScene(nextScene);
		break;
	case Type::VOLUME:
		AudioManager::GetInstance()->setMute(!AudioManager::GetInstance()->getMute());
		break;
	case Type::EXIT:
		OverlayManager::GetInstance()->getMotor()->setStop(true);
		break;
	default:
		break;


	}
}
