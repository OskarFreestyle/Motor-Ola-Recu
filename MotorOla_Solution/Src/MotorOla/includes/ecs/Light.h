#pragma once
#include "Componente.h"
#if _DEBUG
#include "checkML.h"
#endif

// Luces 
class Light : public Componente {
public:
	// An�logos a los tipos de Ogre
	enum LightType { Point, Directional, Spotlight };

	Light() {};
	~Light();
	bool init(const std::map<std::string, std::string>& mapa) override;

	LightType getType();

	float getLinearAttenuation() { return _light->getAttenuationLinear(); }
	float getConstantAttenuation() { return _light->getAttenuationConstant(); }
	float getQuadraticAttenuation() { return _light->getAttenuationQuadric(); }

	bool getCastShadows() { return _light->getCastShadows(); }

	float getSpotlightInnerAngle() { return _light->getSpotlightInnerAngle().valueDegrees(); };
	float getSpotlightOuterAngle() { return _light->getSpotlightOuterAngle().valueDegrees(); };

	void setType(LightType type);

	/// <summary>
	/// Pone la atenuaci�n de la luz de punto
	/// </summary>
	/// <param name="att">El vector 4 con el rango de la luz y las atenuaciones constante, lineal y cuadr�ticas</param>
	void setAttenuation(float r, float c, float l, float q);

	void setCastShadows(bool toggle) { _light->setCastShadows(toggle); }

	void setLightDiffuse(Ogre::ColourValue diff);

	void setLightSpecular(Ogre::ColourValue diff);

	void setSpotlightInnerAngle(float deg);
	void setSpotlightOuterAngle(float deg);

protected:
	void render() override;

	Ogre::Light* _light;
	Ogre::SceneNode* _lightNode;
};