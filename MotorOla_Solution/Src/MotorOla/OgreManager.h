#pragma once

#ifdef MOTOR_EXPORTS
#define MOTOR_API __declspec(dllexport)
#else
#define MOTOR_API __declspec(dllimport)
#endif

#include <string>

#include <Ogre.h>
#include <OgreBuildSettings.h>
#include <OgreLogManager.h>
#include <OgrePlugin.h>
#include <OgreFileSystemLayer.h>
#include <OgreFrameListener.h>
#include <OgreOverlayPrerequisites.h> 
#include <SDL.h> 

namespace Ogre {
	class FileSystemLayer;
	class OverlaySystem;
	class Root;
	class RenderWindow;
	class SceneManager;
}

typedef SDL_Window NativeWindowType;

struct NativeWindowPair
{
	Ogre::RenderWindow* render = nullptr;
	NativeWindowType* native = nullptr;
};

//class OgreManager : public Singleton<OgreManager>, Ogre::FrameListener {	// Creo que esto va a ser InputManager
class MOTOR_API OgreManager {
public:
	/// <summary>
	/// Destructor de la clase
	/// </summary>
	~OgreManager();

	/// <summary>
	/// Devuelve una instancia de la clase.
	/// </summary>
	inline static OgreManager* GetInstance() { return _singleton; }

	/// <summary>
	/// Inicializa la clase SceneManager con los parametros dados si no se ha inicializado antes.
	/// Devuelve true si se inicializa por primera vez y false si ya habia sido inicializada.
	/// </summary>
	static bool Init();

	// Getters
	Ogre::Camera* getCam() const { return cam; };
	Ogre::SceneNode* getCamNode() const { return camNode; };
	Ogre::RenderWindow* getRenderWindow() const { return _window.render; }
	Ogre::SceneManager* getSceneManager() const { return _sceneManager; }
	Ogre::Viewport* getViewPort() const { return _vp; }
	Ogre::Root* getRoot() const { return _root; }
	Ogre::OverlaySystem* getOverlaySystem() const { return _overlaySystem; }
	int getWindowWidth() const { return _window.render->getWidth(); }
	int getWindowHeight() const { return _window.render->getHeight(); }

	void initOgre();
	void update();
	void close();

	// callback interface copied from various listeners to be used by ApplicationContext
	
	virtual void windowMoved(Ogre::RenderWindow* rw) {}
	virtual void windowResized(Ogre::RenderWindow* rw) {}
	virtual bool windowClosing(Ogre::RenderWindow* rw) { return true; }
	virtual void windowClosed(Ogre::RenderWindow* rw) {}
	virtual void windowFocusChange(Ogre::RenderWindow* rw) {}

	// Funciones
	void setup();
	void createRoot();
	bool oneTimeConfig();
	void setWindowGrab(bool grab);
	void locateResources();
	void loadResources();
	void shutdown();

	virtual NativeWindowPair createWindow(const Ogre::String& name);

protected:
	static OgreManager* _singleton;

	/// <summary>
	/// Inicia los parámetros de la clase
	/// </summary>
	/// <param name="appName"> Nombre que aparecerá en la ventana (el del juego) </param>
	OgreManager(const Ogre::String& appName = "MotorOla");

private:
	// Variables
	Ogre::Root* _root = nullptr;
	NativeWindowPair _window;
	Ogre::FileSystemLayer* _fileSystemLayer = nullptr;
	Ogre::OverlaySystem* _overlaySystem=nullptr;  // Overlay system
	Ogre::SceneManager* _sceneManager = nullptr;
	Ogre::Viewport* _vp = nullptr;
	Ogre::Camera* cam = nullptr;
	Ogre::SceneNode* camNode = nullptr;
	bool _firstRun;
	Ogre::String _appName = "MotorOla";
	Ogre::String _solutionPath;
};

