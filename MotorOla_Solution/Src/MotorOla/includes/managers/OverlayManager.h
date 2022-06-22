#pragma once

#ifdef MOTOR_EXPORTS
#define MOTOR_API __declspec(dllexport)
#else
#define MOTOR_API __declspec(dllimport)
#endif
#if _DEBUG
#include "checkML.h"
#endif


#include <string>
#include <Ogre.h>
#include "OverlayManager.h"

#include <OgreOverlaySystem.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreFontManager.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgreOverlayContainer.h>
#include "OgreOverlayPrerequisites.h"
#include <OgrePanelOverlayElement.h>
#include <OgreOverlay.h>

#include <vector>
class Motor;
class OgreManager;
using CallBackOnClick = void(Motor* m);
//using CallBackOnClick = void();

class MOTOR_API OverlayManager {
	
public:
	virtual ~OverlayManager();

	/// <summary>
	/// Devuelve una instancia de la clase.
	/// </summary>
	inline static OverlayManager* GetInstance() { return _singleton; }

	/// <summary>
	/// Inicializa la clase SceneManager con los parametros dados si no se ha inicializado antes.
	/// Devuelve true si se inicializa por primera vez y false si ya habia sido inicializada.
	/// </summary>
	static bool Init(OgreManager* om_, Motor* m);

	void initOverlay(OgreManager* om_, Motor* m);

	//void update();

	void creaBoton(float x,float y,const std::string& texto, const std::string& nombrePanel, const std::string& nombreTexto,float tamLetra,const std::string& material,float dimX,float dimY/*, CallBackOnClick* click_*/);
	void creaTexto(float x, float y, const std::string& texto, const std::string& nombreTexto, float tamLetra,const std::string& nombrePanel,float dimX,float dimY);
	void creaPanel(float x, float y,  const std::string& nombrePanel, const std::string& material, float dimX, float dimY);
	Ogre::TextAreaOverlayElement* getTexto(std::string panelName, std::string textName);

	/// <summary>
	/// Solo cambia a rojo o negro
	/// </summary>
	/// <param name="panelName"></param>
	/// <param name="textName"></param>
	/// <param name="newColor">"Red" para rojo y ya</param>
	/// <returns></returns>
	void changeTextColor(std::string panelName, std::string textName, std::string newColor);
	Ogre::PanelOverlayElement* getPanel(std::string name);
	Ogre::PanelOverlayElement* getBoton(std::string name);
	void clear();
	Motor* getMotor();

protected:
	static OverlayManager* _singleton;

	OverlayManager() {};
private:
	

	std::vector< Ogre::PanelOverlayElement*>botones;
	std::vector< Ogre::PanelOverlayElement*>paneles;
	std::vector< Ogre::PanelOverlayElement*>textos;
	Ogre::Overlay* mOverlay=nullptr;
	
	OgreManager* og;
	Motor* motor;
};


