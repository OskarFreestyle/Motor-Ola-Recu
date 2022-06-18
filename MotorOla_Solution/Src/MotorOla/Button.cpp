#include "Button.h"
#include "OverlayManager.h"
#include "SceneManager.h"
#include "AudioManager.h"
#include "Motor.h"

std::string nS = " ";


bool Button::init(const std::map<std::string, std::string>& mapa) {
	if (mapa.find("positionX") == mapa.end() || mapa.find("positionY") == mapa.end() || mapa.find("texto") == mapa.end() || mapa.find("nombrePanel") == mapa.end() ||
		mapa.find("nombreTexto") == mapa.end() || mapa.find("tamLetra") == mapa.end() || mapa.find("material") == mapa.end()||mapa.find("dimensionX")==mapa.end()||mapa.find("dimensionY")==mapa.end(),
		mapa.find("type")==mapa.end(),mapa.find("nextScene")==mapa.end()) return false;

	

	std::string s = mapa.at("positionX");
	x = std::stof(s);
	s = mapa.at("positionY");
	y = std::stof(s);

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
	
	OverlayManager::GetInstance()->creaBoton(x, y, texto, nombrePanel, nombreTexto, tamLetra, material, dimX, dimY);
	s = mapa.at("type");
	if (s == "CHANGE_SCENE") {
		t = Type::CHANGE_SCENE;
		s = mapa.at("nextScene");
		nextScene = s;
		nS = nextScene;
		OverlayManager::GetInstance()->setCallBackToButton(nombrePanel,changeScene);
		
	}
	else if (s == "VOLUME") {
		t = Type::VOLUME;
		OverlayManager::GetInstance()->setCallBackToButton(nombrePanel, volume);
	}
	else if (s == "EXIT") {
		t = Type::EXIT;
		OverlayManager::GetInstance()->setCallBackToButton(nombrePanel,exit);

	}
	_inicializado = true;

	return true;
}

void Button::changeScene(Motor* m)
{
	OverlayManager::GetInstance()->clear();
	SceneManager::GetInstance()->newScene(nS);
	//SceneManager::GetInstance()->loadEntities();
}
 void Button::volume(Motor*m) {
	if (AudioManager::GetInstance()->getMute()) {
		AudioManager::GetInstance()->setMute(false);
	}
	else {
		AudioManager::GetInstance()->setMute(true);
	}
	
}

void Button::exit(Motor* m)
{
	OverlayManager::GetInstance()->getMotor()->setStop(true);
	
}
