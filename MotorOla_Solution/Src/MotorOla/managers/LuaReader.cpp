#include "LuaReader.h"

// Clases propias
#include "SceneManager.h"
#include "OgreManager.h"
#include "PhysxManager.h"

extern "C"
{
    #include "lua.h"
    #include "lauxlib.h"
    #include "lualib.h"
}

static const luaL_Reg lualibs[] = {
		{ "base",       luaopen_base },
		{ NULL,         NULL }
};

void openlualibs(lua_State* l) {
	const luaL_Reg* lib;

	for (lib = lualibs; lib->func != NULL; lib++) {
		lib->func(l);
		lua_settop(l, 0);
	}
}

LuaReader* LuaReader::_singleton = nullptr;

bool LuaReader::Init()
{
	// Si ya existe devuelve false
	if (_singleton != nullptr) return false;

	// Si lo tiene que crear devuelve true
	_singleton = new LuaReader();

	return true;
}

void LuaReader::readScene(std::string file) {
	// Preparamos un LuaState para leer el fichero
	lua_State* l;
	l = luaL_newstate();
	openlualibs(l);

	// Intenta abrir el archivo .lua y lanzamos excepcion si hay error
	if (!luaL_loadfile(l, file.c_str()) &&  lua_pcall(l, 0, 0, 0)) {
		std::cout << lua_tostring(l, -1) << "\n";
		std::cout << "Error reading .lua\n";
		throw std::exception("Lua file was not able to be loaded\n");
	}

	// Intenta leer la escena
	try {
		// Busca el nivel
		lua_getglobal(l, "GetLevel");

		// Si no encuentra la function que devuelve la tabla
		if (lua_pcall(l, 0, 1, 0) != LUA_OK) {
			std::cout << lua_tostring(l,-1) << "\nError reading GetLevel in .lua\n";	
			throw std::exception("Lua function GetLevel was not able to be loaded");
		}

		// Variables auxiliares para leer la escena
		std::string auxString;
		std::string::size_type sz = 0, sa = 0;
		float a, b, c;

		// Luz Ambiente
		lua_getfield(l, -1, "ambient");
		auxString = lua_tostring(l, -1);
		a = std::stof(auxString, &sz); b = std::stof(auxString.substr(sz + 1), &sa); c = std::stof(auxString.substr(sz + sa + 2));
		OgreManager::GetInstance()->getSceneManager()->setAmbientLight(Ogre::ColourValue(a, b, c));
		lua_pop(l, 1);

		// Gravedad
		lua_getfield(l, -1, "gravity");
		auxString = lua_tostring(l, -1);
		sz = 0, sa = 0;
		a = std::stof(auxString, &sz); b = std::stof(auxString.substr(sz + 1), &sa); c = std::stof(auxString.substr(sz + sa + 2));
		pm().getScene()->setGravity(PxVec3(a, b, c));
		lua_pop(l, 1);

		// Vector de entidades que creamos y vector auxiliar para marcarlas iniciadas
		std::vector<Entidad*> ents;
		std::vector<bool> entInits;

		// Después lee todas las entidades y los componentes de cada una
		lua_getfield(l, -1, "entidades");
		lua_pushnil(l);
		while (lua_next(l, 2) != 0) {
			// Entity is here
			// Name
			lua_getfield(l, -1, "name");
			char* name = (char*)lua_tostring(l, -1);
			lua_pop(l, 1);

			// ID
			lua_getfield(l, -1, "id");
			int id = lua_tonumber(l, -1);
			lua_pop(l, 1);


			//Entidad* ent = Singleton<EntidadManager>::instance()->addEntidad(name, id);
			Entidad* ent = new Entidad(name, id);
			ents.push_back(ent);
			entInits.push_back(false);
			SceneManager::GetInstance()->addEntity(ent);

			// Despues lee los componentes
			lua_getfield(l, -1, "components");
			lua_pushnil(l);
			while (lua_next(l, 5) != 0) { // stack: mapa-entities-indEntity-Entity-compTabla

				char* compName = (char*)lua_tostring(l, -2);

				std::map<std::string, std::string> compMap;
				lua_pushnil(l);

				// Lee cada atributo del componente y genera un mapa
				while (lua_next(l, 7) != 0) {
					char* attrName = (char*)lua_tostring(l, -2);
					std::string s1(attrName);
					char* attrValue = (char*)lua_tostring(l, -1);
					std::string s2(attrValue);
					compMap.insert((std::pair<std::string, std::string>(s1, s2)));
					lua_pop(l, 1);
				}

				// Añade el componente a la entidad
				ent->addComponent(compName, compMap);
				lua_pop(l, 1);
			}

			lua_pop(l, 1);
			lua_pop(l, 1);
		}

		lua_pop(l, 2);

		// Cierra el LuaState
		lua_close(l);

		// Bucle para inicializar las entidades y sus componentes
		int i = 0;
		int numEnts = ents.size();
		int initedEnts = 0;
		while (initedEnts != numEnts) {
			if (!entInits[i] && ents[i]->init()) {
				++initedEnts;
				entInits[i] = true;
			}
			++i;
			i %= numEnts;
		}
	}
	catch (...) {
		throw std::exception("Error leyendo una escena de lua\n");
	}
}

Entidad* LuaReader::readPrefab(std::string file) {
	// Preparamos un LuaState para leer el fichero
	lua_State* l;
	l = luaL_newstate();
	openlualibs(l);

	// Intenta abrir el archivo .prefab y lanzamos excepcion si hay error
	if (!luaL_loadfile(l, file.c_str()) && lua_pcall(l, 0, 0, 0)) {
		throw std::exception("Lua prefab was not able to be loaded");
	}

	// Intenta leer el prefab
	try {

		lua_getglobal(l, "GetPrefab");
		int err = lua_pcall(l, 0, 1, 0);

		lua_getfield(l, -1, "name");
		char* name = (char*)lua_tostring(l, -1);
		lua_pop(l, 1);

		lua_getfield(l, -1, "id");
		int id = lua_tonumber(l, -1);
		lua_pop(l, 1);		

		Entidad* ent = new Entidad(name, id);
		SceneManager::GetInstance()->addEntity(ent);

		// Lee los componentes
		lua_getfield(l, -1, "components");
		lua_pushnil(l);
		while (lua_next(l, 2) != 0) { // stack: Entity-compTabla

			char* compName = (char*)lua_tostring(l, -2);

			std::map<std::string, std::string> compMap;
			lua_pushnil(l);
			while (lua_next(l, 4) != 0) { // stack: Entity-compTabla-indComp-Component
				char* attrName = (char*)lua_tostring(l, -2);
				std::string s1(attrName);
				char* attrValue = (char*)lua_tostring(l, -1);
				std::string s2(attrValue);
				compMap.insert((std::pair<std::string, std::string>(s1, s2)));
				lua_pop(l, 1);
			}

			// Funcion de traduccion
			ent->addComponent(compName, compMap);
			lua_pop(l, 1);
		}

		lua_pop(l, 1);

		lua_close(l);

		// Inicia la entidad y sus componentes
		ent->init();

		return ent;
	}
	catch (...) {
		throw std::exception("Prefab file has incorrect formatting");
	}
}



