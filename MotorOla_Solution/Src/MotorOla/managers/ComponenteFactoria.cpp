#include "ComponenteFactoria.h"

ComponenteFactoria* ComponenteFactoria::_singleton = nullptr;

bool ComponenteFactoria::Init() {
	// Si ya existe devuelve false
	if (_singleton != nullptr) return false;

	// Si lo tiene que crear devuelve true
	_singleton = new ComponenteFactoria();
	return true;
}

Componente* ComponenteFactoria::getComponent(std::string name)
{
	auto it = _mGenerators.find(name);
	if (it != _mGenerators.end())
	{
		return it->second();
	}
	return nullptr;
}

bool ComponenteFactoria::registerGenerator(std::string compName, const componentInstanceGenerator& instGenerator)
{
	return _mGenerators.insert(std::make_pair(compName, instGenerator)).second;
}