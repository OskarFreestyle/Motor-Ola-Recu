#include "Entidad.h"
#include "Transform.h"
#include "LoadResources.h"
#include "PhysxManager.h"
#include "LuaReader.h"
#include "RigidBody.h"
#include "Mesh.h"
#include "ComponenteFactoria.h"


Entidad::Entidad() : 
	_name("Entity"), _id(), _tag("Default"), _paused(false)
{
}

Entidad::Entidad(std::string entityName, int id, std::string entityTag) :
	_name(entityName), _tag(entityTag), _id(id), _paused(false)
{
}

Entidad::~Entidad()
{
	//int n = components.size();
	//for (int i = 0; i < n; i++)
	//{
	//	if (components.at(i) != nullptr) {
	//		components.at(i).reset();
	//		components.at(i) = nullptr;
	//	}
	//}
}

void Entidad::update()
{
	for (auto& c : components)
	{
		c->update();
	}
}

bool Entidad::isActive() const
{
	return active;
}

void Entidad::destroy()
{
	active = false;
}

void Entidad::OnCollisionEnter(Entidad* other)
{
	if (this!=nullptr && other!=nullptr) {
		for (auto& c : components)
		{
			if (c.get()->_entity->isActive() && other->isActive()) c->onCollisionStart(other);
		}
	}
}

void Entidad::OnTriggerEnter(Entidad* other)
{
	if (this != nullptr && other != nullptr) {
		for (auto& c : components)
		{
			if (c.get()->_entity->isActive() && other->isActive()) c->onTriggerStart(other);
		}
	}
}


Componente* Entidad::addComponent(const std::string& compName, const std::map<std::string, std::string>& map) {
	Componente* t = ComponenteFactoria::GetInstance()->getComponent(compName);
	if (t != nullptr) {
		t->_entity = this;//ponemos la entidad en el componente
		std::unique_ptr<Componente> upt(t);
		components.push_back(std::move(upt));
		compMaps.push_back(map);
		compinits.push_back(false);

		return t;
	}
	throw std::exception("Error de carga del componente ");
}

inline void Entidad::setActive(bool state)
{
	active = state;
	if (hasComponent<Mesh>()) getComponent<Mesh>()->setVisible(state);
}

bool Entidad::init()
{
	_numTriesToLoad = components.size() * 100000;
	int i = 0;
	j = 0;
	int numComponents = components.size();
	int initedComps = 0;
	while (initedComps != numComponents && j < _numTriesToLoad) {
		if (!compinits[i] && components[i]->init(compMaps[i])) {
			++initedComps;
			compinits[i] = true;
		}
		if (_needsOtherEntity) {
			return false;
		}
		++i;
		j++;
		i %= numComponents;
	}
	if (j >= _numTriesToLoad) {
		throw std::exception("Error al iniciar los componentes en Entity \n ");
	}
	Transform* t;
	i = 0;
	while (i < components.size() && !dynamic_cast<Transform*> (components[i].get())) {
		i++;
	}
	if (i != 0 && i < components.size()) {
		components[0].swap(components[i]);
	}
	return true;
}

Entidad* Entidad::instantiate(std::string name, Vectola3D position, Quaterniola rotation)
{
	std::string path = LoadResources::GetInstance()->prefab(name);
	Entidad* ent = readPrefab(path);
	ent->getComponent<Transform>()->setPosition(position);
	ent->getComponent<Transform>()->setRotation(rotation);
	if (ent->getComponent<RigidBody>() != nullptr) {
		pm().setGlobalToPhysxTR(*ent, *ent->getComponent<RigidBody>()->getBody());
	}

	return ent;
}
