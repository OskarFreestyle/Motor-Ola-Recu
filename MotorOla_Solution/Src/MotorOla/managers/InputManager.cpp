#include "InputManager.h"

InputManager* InputManager::_singleton = nullptr;

bool InputManager::Init() {
	// Si ya existe devuelve false
	if (_singleton != nullptr) return false;

	// Si lo tiene que crear devuelve true
	_singleton = new InputManager();
	return true;
}
