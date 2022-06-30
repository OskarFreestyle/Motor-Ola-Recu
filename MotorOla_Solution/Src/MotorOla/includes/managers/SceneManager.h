#pragma once
#ifdef MOTOR_EXPORTS
#define MOTOR_API __declspec(dllexport)
#else
#define MOTOR_API __declspec(dllimport)
#endif

#include "Entidad.h"

// Gestor de las entidades en la escena
class MOTOR_API SceneManager {
public:
	~SceneManager();

	/// <summary>
	/// Devuelve una instancia de la clase.
	/// </summary>
	inline static SceneManager* GetInstance() { return _singleton; }

	/// <summary>
	/// Inicializa la clase SceneManager con los parametros dados si no se ha inicializado antes.
	/// Devuelve true si se inicializa por primera vez y false si ya habia sido inicializada.
	/// </summary>
	static bool Init();

	void updateEntidades();

	/// <summary>
	/// Añade una entidad a la lista. Si existe devuelve false, sino, true y la añade.
	/// </summary>
	bool addEntity(Entidad* ent);

	/// <summary>
	/// Añade una entidad a la lista de entidades a eliminar. Si existe devuelve false, sino, true y la añade.
	/// </summary>
	bool addEntityToRemove(Entidad* ent);

	/// <summary>
	/// Devuelve una entidad dado su id, si no la encuentra devuelve nullptr. Busca en la lista de la escena y/o en la lista de entidades permanentes
	/// </summary>
	Entidad* getEntityByID(int id);

	/// <summary>
	/// Devuelve una entidad dado su nombre, si no la encuentra devuelve nullptr. Busca en la lista de la escena y/o en la lista de entidades permanentes
	/// </summary>
	Entidad* getEntityByName(const std::string& name);

	/// <summary>
	/// Elimina una entidad de la lista y la destruye.
	/// </summary>
	void removeEntities();

	/// <summary>
	/// Devuelve una lista con todas las entidades creadas.
	/// </summary>
	inline std::vector<Entidad*>* getEntities() { return &_entidades; }

	/// <summary>
	/// Avisa al manager de que se va a cargar una escana
	/// </summary>
	void newScene(std::string sceneName);

	/// <summary>
	/// Carga las entidades que hay en el vector de entidades a cargar en el vector de entidades a usar
	/// </summary>
	void loadEntities();

	/// <summary>
	/// Pausa las entidades no permanentes
	/// </summary>
	void pauseScene();

	/// <summary>
	/// Reanuda las entidades pausadas, las no pausadas las elimina
	/// </summary>
	void continueScene();

	/// <summary>
	/// Elimina las entidades en las listas.
	/// </summary>
	void deleteEntities();

	// Devuelve el nombre de la escena actual
	inline std::string getCurrentScene() { return _sceneName; }

protected:
	static SceneManager* _singleton;
	
	SceneManager() {};	// Constructor vacío

	bool _newScene = false;			// Se marca para el cambio de escena
	std::string _sceneNameRoute;	// La ruta de la escena
	std::string _sceneName;			// El nombre de la escena como tal

	std::vector<Entidad*> _entidades;			// Vector con las entidades que hay
	std::vector<Entidad*> _entidadesToRemove;	// Vector con las entidades a borrar al final del frame
	std::vector<Entidad*> _entidadesToLoad;		// Vector con las entidades a cargar al final del frame
};