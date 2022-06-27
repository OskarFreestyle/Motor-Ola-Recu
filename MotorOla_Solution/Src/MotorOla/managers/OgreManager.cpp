#include "OgreManager.h"

#include <OgreRoot.h>
#include <OgreGpuProgramManager.h>
#include <OgreConfigFile.h>
#include <OgreRenderWindow.h>
#include <OgreViewport.h>
#include <OgreDataStream.h>
#include <OgreFileSystemLayer.h>
#include <OgreOverlaySystem.h>
#include <OgreOverlayManager.h>

#include <SDL_video.h>
#include <SDL_syswm.h>

#include "InputManager.h"

OgreManager* OgreManager::_singleton = nullptr;

OgreManager::OgreManager(const Ogre::String& appName)
{
	_appName = appName;
	_fileSystemLayer = new Ogre::FileSystemLayer(appName);
	_root = nullptr;
	_overlaySystem = nullptr;
	_firstRun = true;
}

OgreManager::~OgreManager()
{
	close();
	delete _fileSystemLayer;
}

bool OgreManager::Init() {
	// Si ya existe devuelve false
	if (_singleton != nullptr) return false;

	// Si lo tiene que crear devuelve true
	_singleton = new OgreManager();
	
	try {
		_singleton->initOgre();
	}
	catch (...) { throw std::exception("Error iniciando OgreManager\n"); }


	return true;
}

void OgreManager::initOgre()
{
    // Crea el root de Ogre
    createRoot();
	
    // Inicia la ventana
	if (oneTimeConfig()) {
		setup();
	}

	// Agregado una prueba
	_sceneManager = _root->createSceneManager();

	_root->getSceneManager("SceneManagerInstance1")->addRenderQueueListener(_overlaySystem);
}

void OgreManager::update()
{
	// Renderiza el frame actual
	_root->renderOneFrame();
}

void OgreManager::close()
{
	if (_root != nullptr)
	{
		_root->saveConfig();
	}
	shutdown();
	delete _root;
	_root = nullptr;
}

void OgreManager::createRoot()
{
	Ogre::String pluginsPath;
	pluginsPath = _fileSystemLayer->getConfigFilePath("plugins.cfg");

	// Se comprueba la existencia de plugins.cfg
	if (!Ogre::FileSystemLayer::fileExists(pluginsPath))
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_FILE_NOT_FOUND, "plugins.cfg", "Error buscando el archivo plugins.cfg");
	}

	_solutionPath = pluginsPath;    //  añadido para definir directorios relativos al de la solución 
	_solutionPath.erase(_solutionPath.find_last_of("\\") + 1, _solutionPath.size() - 1);
	_fileSystemLayer->setHomePath(_solutionPath);   //  para los archivos de configuración ogre. (en el bin de la solubión)
	_solutionPath.erase(_solutionPath.find_last_of("\\") + 1, _solutionPath.size() - 1);   //  Quito /bin

	_root = new Ogre::Root(pluginsPath, _fileSystemLayer->getWritablePath("ogre.cfg"), _fileSystemLayer->getWritablePath("ogre.log"));

	_root->setRenderSystem(_root->getRenderSystemByName("OpenGL Rendering Subsystem"));
}

void OgreManager::shutdown()
{
	ih().refresh();

	// Borra la ventana
	if (_window.render != nullptr)
	{
		_root->destroyRenderTarget(_window.render);
		_window.render = nullptr;
	}

	// Borra el sistema Overlay

	if (Ogre::OverlayManager::getSingletonPtr() != nullptr) {
		delete _overlaySystem;
		_overlaySystem = nullptr;
	}


	if (_window.native != nullptr)
	{
		
		SDL_DestroyWindow(_window.native);
		_window.native = nullptr;
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
			//SDL_Quit();
		
		
	}
}

void OgreManager::setup()
{
	_root->initialise(false);
	createWindow(_appName);
	setWindowGrab(false);
	//Crea el overlay System antes de cargar los recursos para cargar la fuente en el FontManager
	_overlaySystem = new Ogre::OverlaySystem();
	
	locateResources();
	loadResources();
}

bool OgreManager::oneTimeConfig()
{
	if (!_root->restoreConfig())
	{
		return _root->showConfigDialog(NULL);
	}
	else return true;
}



NativeWindowPair OgreManager::createWindow(const Ogre::String& name)
{
    uint32_t w, h;
    Ogre::NameValuePairList miscParams;

    Ogre::ConfigOptionMap ropts = _root->getRenderSystem()->getConfigOptions();

    std::istringstream mode(ropts["Video Mode"].currentValue);
    Ogre::String token;
    mode >> w; // width
    mode >> token; // 'x' as seperator between width and height
    mode >> h; // height

    miscParams["FSAA"] = ropts["FSAA"].currentValue;
    miscParams["vsync"] = ropts["VSync"].currentValue;
    miscParams["gamma"] = ropts["sRGB Gamma Conversion"].currentValue;

    if (!SDL_WasInit(SDL_INIT_VIDEO)) SDL_InitSubSystem(SDL_INIT_VIDEO);

    Uint32 flags = SDL_WINDOW_RESIZABLE;

    if (ropts["Full Screen"].currentValue == "Yes")  flags = SDL_WINDOW_FULLSCREEN;
    //else  flags = SDL_WINDOW_RESIZABLE;

    _window.native = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, flags);

    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(_window.native, &wmInfo);

    miscParams["externalWindowHandle"] = Ogre::StringConverter::toString(size_t(wmInfo.info.win.window));

    _window.render = _root->createRenderWindow(name, w, h, false, &miscParams);
    return _window;
}

void OgreManager::setWindowGrab(bool _grab)
{
	SDL_bool grab = SDL_bool(_grab);
	SDL_SetWindowGrab(_window.native, grab);
	//SDL_ShowCursor(grab);
	SDL_ShowCursor(true);
}



void OgreManager::loadResources()
{
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void OgreManager::locateResources()
{
	// load resource paths from config file
	Ogre::ConfigFile cf;

	Ogre::String resourcesPath = _fileSystemLayer->getConfigFilePath("resources.cfg");
	if (Ogre::FileSystemLayer::fileExists(resourcesPath))
	{
		cf.load(resourcesPath);
	}
	else
	{
		// Lee la carpeta de modelos
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
			Ogre::FileSystemLayer::resolveBundlePath(_solutionPath + "\\Assets\\models"),
			"FileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		// Lee la carpeta de materiales
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
			Ogre::FileSystemLayer::resolveBundlePath(_solutionPath + "\\Assets\\materials"),
			"FileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		// Lee la carpeta de materiales
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
			Ogre::FileSystemLayer::resolveBundlePath(_solutionPath + "\\Assets\\textures"),
			"FileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
			Ogre::FileSystemLayer::resolveBundlePath(_solutionPath + "\\Assets\\fonts"),
			"FileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		
	}
	
	Ogre::String sec, type, arch;
	// go through all specified resource groups
	Ogre::ConfigFile::SettingsBySection_::const_iterator seci;
	for (seci = cf.getSettingsBySection().begin(); seci != cf.getSettingsBySection().end(); ++seci) {
		sec = seci->first;
		const Ogre::ConfigFile::SettingsMultiMap& settings = seci->second;
		Ogre::ConfigFile::SettingsMultiMap::const_iterator i;

		// go through all resource paths
		for (i = settings.begin(); i != settings.end(); i++)
		{
			type = i->first;
			arch = Ogre::FileSystemLayer::resolveBundlePath(i->second);
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch, type, sec);
		}
	}

	sec = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
	const Ogre::ResourceGroupManager::LocationList genLocs = Ogre::ResourceGroupManager::getSingleton().getResourceLocationList(sec);

	

	arch = genLocs.front().archive->getName();
	type = genLocs.front().archive->getType();

	// Add locations for supported shader languages
	if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsles"))
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/materials/programs/GLSLES", type, sec);
	}
	else if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl"))
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/materials/programs/GLSL120", type, sec);

		if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl150"))
		{
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/materials/programs/GLSL150", type, sec);
		}
		else
		{
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/materials/programs/GLSL", type, sec);
		}

		if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl400"))
		{
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/materials/programs/GLSL400", type, sec);
		}
	}
	else if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("hlsl"))
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/materials/programs/HLSL", type, sec);
	}

	Ogre::String mRTShaderLibPath = arch + "/RTShaderLib";
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mRTShaderLibPath + "/materials", type, sec);
	
	if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsles"))
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mRTShaderLibPath + "/GLSL", type, sec);
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mRTShaderLibPath + "/GLSLES", type, sec);
	}
	else if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl"))
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mRTShaderLibPath + "/GLSL", type, sec);
	}
	else if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("hlsl"))
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mRTShaderLibPath + "/HLSL", type, sec);
	}
}
