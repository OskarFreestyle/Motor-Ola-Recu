#pragma once
#ifdef MOTOR_EXPORTS
#define MOTOR_API __declspec(dllexport)
#else
#define MOTOR_API __declspec(dllimport)
#endif 
#if _DEBUG
#include "checkML.h"
#endif
#include "Componente.h"


class Camera : public Componente
{
public:
	enum ProjectionType { Orthographic, Perspective };
	Camera();
	~Camera();
	bool init(const std::map<std::string, std::string>& mapa) override;

	float getNearClipPlane() { return _cam->getNearClipDistance(); }
	float getFarClipPlane() { return _cam->getFarClipDistance(); }
	float getAspectRatio() { return _cam->getAspectRatio(); }
	float getFOVY() { return _cam->getFOVy().valueDegrees(); }
	ProjectionType getProjectionType();

	void setNearClipPlane(float clip) { _cam->setNearClipDistance(clip); }
	void setFarClipPlane(float clip) { _cam->setFarClipDistance(clip); }
	void setAspectRatio(bool autoAspectRatio, float aspectRatio = 1.0f);
	void setFOVY(float fov);
	void setProjectionType(ProjectionType type);
	void setViewport(float posX, float posY, float sizeX, float sizeY);
	void setBackgroundColor(Vectola3D color);
	void setBackgroundColor(float r, float g, float b);
protected:
	void render() override;

	Ogre::Viewport* _viewport;
	Ogre::Camera* _cam;
	Ogre::SceneNode* _camNode;
};