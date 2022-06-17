#include "SceneManager.h"
#include "LoadResources.h"
#include "LuaReader.h"
#include "PhysxManager.h"

SceneManager* SceneManager::_singleton = nullptr;

SceneManager::~SceneManager() {
	// Vaciamos todos los vectores de entidades
	for (Entidad* e : _entidades) if (e != nullptr) delete e;
	for (Entidad* r : _entidadesToRemove) if (r != nullptr) delete r;
	for (Entidad* l : _entidadesToLoad) if (l != nullptr) delete l;
}

bool SceneManager::Init() {
	// Si ya existe devuelve false
	if (_singleton != nullptr) return false;

	// Si lo tiene que crear devuelve true
	_singleton = new SceneManager();
	return true;
}

void SceneManager::updateEntidades()
{
	// Para todas las entidades
	for (Entidad* e : _entidades) {
		// Si no esta pausada se actualiza (actualiza sus todos sus componentes) 
		if (!e->getPaused()) {
			//cout << "Entidad actualizada"<<e->getName()<<"\n";
			e->update();
		}
		//else cout << "Entidad pausada\n";
	}

}

bool SceneManager::addEntity(Entidad* ent) {
	// Primero comprueba que la entidad no exista 
	auto it = find(_entidades.begin(), _entidades.end(), ent);
	if (it != _entidades.end()) return false;

	// Luego se comprueba que la entidad no este en la lista de entidades a crear
	it = find(_entidadesToLoad.begin(), _entidadesToLoad.end(), ent);
	if (it != _entidadesToLoad.end()) return false;

#if (defined _DEBUG)
	std::cout << "Entidad añadida\n";
#endif

	// Despues añade esa entidad a la lista de entidades que crear
	_entidadesToLoad.push_back(ent); return true;
}

bool SceneManager::addEntityToRemove(Entidad* ent) {
	auto it = find(_entidades.begin(), _entidades.end(), ent);
	if (it == _entidades.end()) return false;

	//Se cambian las entidades al vector de eliminar
	_entidadesToRemove.push_back(*it);
	_entidades.erase(it);
	return true;
}

Entidad* SceneManager::getEntityByID(int id)
{
	// Busca entre las entidades activas
	Entidad* ent = nullptr;
	auto it = _entidades.begin();
	while (ent == nullptr && it != _entidades.end()) {
		if ((*it)->getID() == id)
			ent = *it;
		++it;
	}

	//Si no se encuentra, entre las entidades a crear
	if (ent == nullptr) {
		auto it = _entidadesToLoad.begin();
		while (ent == nullptr && it != _entidadesToLoad.end()) {
			if ((*it)->getID() == id)
				ent = *it;
			++it;
		}
	}
	return ent;
}

void SceneManager::removeEntities() {
	// Borra todas las entidades marcadas
	for (Entidad* e : _entidadesToRemove) {
		if (e) delete e;
	}
	_entidadesToRemove.clear();
}

void SceneManager::deleteEntities() {
	//Elimina todas las entidades creadas y a crear
	for (Entidad* e : _entidades) {
		_entidadesToRemove.push_back(e);
	}
	for (Entidad* e : _entidadesToLoad) {
		_entidadesToRemove.push_back(e);
	}
	_entidades.clear();
	_entidadesToLoad.clear();
}


void SceneManager::newScene(std::string sceneName) {
	// Primero borra todas las entidades de esta escena
	deleteEntities();
	// Cambia el nombre de la escena
	_sceneName = sceneName;
	// Buscar la ruta de dicha escena (el scene.lua)
	_sceneNameRoute = LoadResources::GetInstance()->scene(sceneName);
	// Marca que hay una nueva escena para que se cargue al frame siguiente
	_newScene = true;
}

void SceneManager::loadEntities() {
	// Añade las entidades de la nueva escena al vector de entidades a crear
	if (_newScene) {
		LuaReader::GetInstance()->readFile(_sceneNameRoute);
		_newScene = false;
	}

	// Crea dichas entidades
	for (Entidad* e : _entidadesToLoad)
		_entidades.push_back(e);
	_entidadesToLoad.clear();
}

void SceneManager::pauseScene() {
	// Marca en pausa todas las entidades de la escena actual
	for (Entidad* e : _entidades) e->setPaused(true);

	// TODO PAUSAR FISICAS
	//PhysxManager::instance()->togglePause();
	//BulletInstance::GetInstance()->setPaused(true);
}

void SceneManager::continueScene() {
	for (Entidad* e : _entidades) {
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