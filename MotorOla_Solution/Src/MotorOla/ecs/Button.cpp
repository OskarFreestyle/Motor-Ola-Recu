#include "Button.h"
#include "OverlayManager.h"
#include "SceneManager.h"
#include "AudioManager.h"
#include "Motor.h"
#include "InputManager.h"
#include "AudioSource.h"
#include "Entidad.h"

Button::~Button() {
	
}

bool Button::init(const std::map<std::string, std::string>& mapa) {
	if (mapa.find("positionX") == mapa.end() || mapa.find("positionY") == mapa.end() || mapa.find("texto") == mapa.end() || mapa.find("nombrePanel") == mapa.end() ||
		mapa.find("nombreTexto") == mapa.end() || mapa.find("tamLetra") == mapa.end() || mapa.find("material") == mapa.end()||mapa.find("dimensionX")==mapa.end()||mapa.find("dimensionY")==mapa.end()||
		mapa.find("type")==mapa.end()||mapa.find("nextScene")==mapa.end()|| mapa.find("clickDelay") == mapa.end()) return false;

	std::string auxString = mapa.at("positionX");
	_posX = std::stof(auxString);
	auxString = mapa.at("positionY");
	_posY = std::stof(auxString);

	auxString = mapa.at("texto");
	_texto = auxString;

	auxString = mapa.at("nombrePanel");
	_nombrePanel = auxString;
	auxString = mapa.at("nombreTexto");
	_nombreTexto = auxString;
	auxString = mapa.at("tamLetra");
	_tamLetra = std::stof(auxString);
	auxString = mapa.at("material");
	_material = auxString;
	auxString = mapa.at("dimensionX");
	_dimX = std::stof(auxString);
	auxString = mapa.at("dimensionY");
	_dimY = std::stof(auxString);
	
	OverlayManager::GetInstance()->creaBoton(_posX, _posY, _texto, _nombrePanel, _nombreTexto, _tamLetra, _material, _dimX, _dimY);
	auxString = mapa.at("type");
	if (auxString == "CHANGE_SCENE") {
		_type = Type::CHANGE_SCENE;
		auxString = mapa.at("nextScene");
		_nextScene = auxString;
	}
	else if (auxString == "VOLUME") {
		_type = Type::VOLUME;
	}
	else if (auxString == "EXIT") {
		_type = Type::EXIT;
	}
	auxString = mapa.at("clickDelay");
	_clickDelay = std::stof(auxString);

	_inicializado = true;

	return _inicializado;
}

void Button::update()
{
	if (!_isClick && isClicked()) {
		// Si tiene algun audio asociado, suena
		if (_entity->hasComponent<AudioSource>()) _entity->getComponent<AudioSource>()->play();
		//Lo marca pulsado
		_isClick = true;
		//Actualiza la variable inClick con el momento pulsado
		_timeClick = clock();
	}
	//Si pasa el tiempo del delay entre la pulsación y la funcion
	if (_isClick && _timeClick+_clickDelay<clock()) {
		//Lo marca a falso
		_isClick = false;
		//Funciones del boton
		onClick();
	}
}


bool Button::isClicked() {
	//DEvuelve si el raton esta dentro del boton
	return (ih().getMouseButtonState(ih().LEFT)) &&
		(ih().getMousePos().first > _posX * OgreManager::GetInstance()->getWindowWidth() && ih().getMousePos().first < (_posX + _dimX)* OgreManager::GetInstance()->getWindowWidth() &&
			ih().getMousePos().second>_posY * OgreManager::GetInstance()->getWindowHeight() && ih().getMousePos().second < (_posY + _dimY)* OgreManager::GetInstance()->getWindowHeight());
}

void Button::onClick()
{
	// Realiza la accion correspondiente
	switch (_type) {
	case Type::CHANGE_SCENE:
		ih().MouseButtonUp(ih().LEFT);
		AudioManager::GetInstance()->stopAllChannels();
		OverlayManager::GetInstance()->clear();
		SceneManager::GetInstance()->newScene(_nextScene);	// Escena pasada por carga de datos
		break;
	case Type::VOLUME:
		ih().MouseButtonUp(ih().LEFT);
		AudioManager::GetInstance()->setMute(!AudioManager::GetInstance()->getMute());//Cambia el mute
		break;
	case Type::EXIT:
		ih().MouseButtonUp(ih().LEFT);
		Motor::GetInstance()->setStop(true);//Sale del bucle del motor
		break;
	default:
		break;
	}
}
