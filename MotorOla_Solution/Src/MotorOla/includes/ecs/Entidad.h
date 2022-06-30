#pragma once
#ifdef MOTOR_EXPORTS
#define MOTOR_API __declspec(dllexport)
#else
#define MOTOR_API __declspec(dllimport)
#endif

#include "Componente.h"
#include "Quaterniola.h"

class Entidad
{
	using uptr_cmp = std::unique_ptr<Componente>;
public:
	Entidad();
	Entidad(std::string entityName, int id, std::string entidadTag = "Default");

	virtual ~Entidad();

	MOTOR_API void update();
	MOTOR_API void draw() {};
	MOTOR_API bool isActive() const;
	MOTOR_API void destroy();

	MOTOR_API void OnCollisionEnter(Entidad* other);
	MOTOR_API void OnTriggerEnter(Entidad* other);

	MOTOR_API inline int getID() { return _id; }

	template<typename T>
	bool hasComponent() {
		return getComponent<T>() != nullptr;
	}

	//template<typename T>
	//bool hasInitComponent() {
	//	return (getComponent<T>() != nullptr && getComponent<T>());
	//}

	/// <summary>
	/// Devuelve un componente de la entidad, o nullptr si no lo tiene. Coste: O(N) :(
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns></returns>
	template<typename T>
	T* getComponent() {
		T* ret = nullptr;
		int i = 0;
		while (i < components.size() && ret == nullptr) {
			ret = dynamic_cast<T*>(components[i].get());
			++i;
		}
		return ret;
	}

	Componente* addComponent(const std::string& compName, const std::map<std::string, std::string>& map);

	// Getters
	MOTOR_API inline std::string getName() const { return _name; }
	MOTOR_API inline std::string getTag() const { return _tag; }
	MOTOR_API inline int getID() const { return _id; }
	MOTOR_API inline bool getPaused() const { return _paused; };

	// Setters
	MOTOR_API inline void setName(std::string n) { _name = n; }
	MOTOR_API inline void setTag(std::string t) { _tag = t; };
	MOTOR_API inline void setID(int i) { _id = i; }
	MOTOR_API inline void setPaused(bool state) { _paused = state; };

	MOTOR_API inline void setActive(bool state);//Quitar

	MOTOR_API bool init();

	MOTOR_API static Entidad* instantiate(std::string name, Vectola3D position = Vectola3D(), Quaterniola rotation = Quaterniola());

private:
	std::string _name;	// Nombre de la entidad
	std::string _tag;	// Tag de la entidad
	int _id;			// Id de la entidad (no tiene porque ser unico)
	bool _paused;		// Si la entidad esta pausada

	std::vector<uptr_cmp> components;	// Vector de componentes de la entidad
	std::vector<bool> compinits;		// Marca los componentes que estan inicializados
	std::vector<std::map<std::string, std::string>> compMaps;
	
	bool active = true;	// Quitar porque esto se va a hacer en el SceneManager


	// Aqui estaran los componentes de esta entidad
	ComponentArray componentArray;
	ComponentBitSet componentBitset;

	bool _needsOtherEntity = false;

	//numero de veces que puedes intentar iniciar tus componentes (para evitar bucle infinito)
	int _numTriesToLoad;
	int j;
};

