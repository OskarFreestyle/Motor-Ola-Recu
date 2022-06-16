#include "SceneManager.h"
#include "LoadResources.h"
#include "LuaReader.h"
#include "PhysxManager.h"

SceneManager* SceneManager::_singleton = nullptr;

SceneManager::~SceneManager() {
	// Vaciamos todos los vectores de entidades
	for (Entidad* e : _entities) if (e != nullptr) delete e;
	for (Entidad* r : _entitiesToRemove) if (r != nullptr) delete r;
	for (Entidad* l : _entitiesToLoad) if (l != nullptr) delete l;
}

bool SceneManager::Init() {
	// Si ya existe devuelve false
	if (_singleton != nullptr) return false;

	// Si lo tiene que crear devuelve true
	_singleton = new SceneManager(); 
	return true;
}

bool SceneManager::addEntity(Entidad* ent) {
	// Primero comprueba que la entidad no exista 
	auto it = find(_entities.begin(), _entities.end(), ent);
	if (it != _entities.end()) return false;

	// Luego se comprueba que la entidad no este en la lista de entidades a crear
	it = find(_entitiesToLoad.begin(), _entitiesToLoad.end(), ent);
	if (it != _entitiesToLoad.end()) return false;

	// Despues añade esa entidad a la lista de entidades que crear
	_entitiesToLoad.push_back(ent); return true;
}

bool SceneManager::addEntityToRemove(Entidad* ent) {
	auto it = find(_entities.begin(), _entities.end(), ent);
	if (it == _entities.end()) return false;

	//Se cambian las entidades al vector de eliminar
	_entitiesToRemove.push_back(*it);
	_entities.erase(it);
	return true;
}

Entidad* SceneManager::getEntityByID(int id)
{
	// Busca entre las entidades activas
	Entidad* ent = nullptr;
	auto it = _entities.begin();
	while (ent == nullptr && it != _entities.end()) {
		if ((*it)->getID() == id)
			ent = *it;
		++it;
	}

	//Si no se encuentra, entre las entidades a crear
	if (ent == nullptr) {
		auto it = _entitiesToLoad.begin();
		while (ent == nullptr && it != _entitiesToLoad.end()) {
			if ((*it)->getID() == id)
				ent = *it;
			++it;
		}
	}
	return ent;
}

void SceneManager::removeEntities() {
	// Borra todas las entidades marcadas
	for (Entidad* e : _entitiesToRemove) {
		if (e) delete e;
	}
	_entitiesToRemove.clear();
}

void SceneManager::deleteEntities() {
	//Elimina todas las entidades creadas y a crear
	for (Entidad* e : _entities) {
		_entitiesToRemove.push_back(e);
	}
	for (Entidad* e : _entitiesToLoad) {
		_entitiesToRemove.push_back(e);
	}
	_entities.clear();
	_entitiesToLoad.clear();
}


void SceneManager::newScene(std::string sceneName) {
	// Primero borra todas las entidades de esta escena
	deleteEntities();
	// Cambia el nombre de la escena
	_sceneName = sceneName;
	// Buscar la ruta de dicha escena (el scene.lua)
	_sceneNameRoute = LoadResources::instance()->scene(sceneName);
	// Marca que hay una nueva escena para que se cargue al frame siguiente
	_newScene = true;
}

void SceneManager::loadEntities() {
	// Añade las entidades de la nueva escena al vector de entidades a crear
	if (_newScene) {
		readFile(_sceneNameRoute);
		_newScene = false;
	}

	// Crea dichas entidades
	for (Entidad* e : _entitiesToLoad)
		_entities.push_back(e);
	_entitiesToLoad.clear();
}

void SceneManager::pauseScene() {
	// Marca en pausa todas las entidades de la escena actual
	for (Entidad* e : _entities) e->setPaused(true);

	// TODO PAUSAR FISICAS
	//PhysxManager::instance()->togglePause();
	//BulletInstance::GetInstance()->setPaused(true);
}

void SceneManager::continueScene() {
	for (Entidad* e : _entities) {
		//Si es una entidad pausada la reanuda
		if (e->getPaused())
			e->setPaused(false);
		//Si no la elimina
		else
			addEntityToRemove(e);
	}

	// TODO REANUDAR FÍSCAS
	//BulletInstance::GetInstance()->setPaused(false);
}