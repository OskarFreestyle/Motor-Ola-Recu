#pragma once

#ifdef MOTOR_EXPORTS
#define MOTOR_API __declspec(dllexport)
#else
#define MOTOR_API __declspec(dllimport)
#endif

#include <PxPhysicsAPI.h>
#include <vector>
#include <cassert>

#include "Entidad.h"
#include "ContactReportCallback.h"
class Transform;

using namespace physx;

MOTOR_API inline std::ostream& operator<<(std::ostream& os, const physx::PxVec3& v) {
	os << "(" << v.x << "," << v.y << "," << v.z << ")";
	return os;
}

MOTOR_API inline std::ostream& operator<<(std::ostream& os, const physx::PxQuat& q) {
	os << "(" << q.x << ", " << q.y << ", " << q.z << ", " << q.w << ")";
	return os;
}

class MOTOR_API PhysxManager {
public:	
	~PhysxManager();

	/// <summary>
	/// Devuelve una instancia de la clase.
	/// </summary>
	inline static PhysxManager* GetInstance() { return _singleton; }

	/// <summary>
	/// Inicializa la clase PhysxManager con los parametros dados si no se ha inicializado antes.
	/// Devuelve true si se inicializa por primera vez y false si ya habia sido inicializada.
	/// </summary>
	static bool Init();

	// main resources
	void runPhysX();
	void togglePause() { mPause != mPause; };

	// transform
	PxTransform globalToPhysxTR(Transform& tr);
	Transform physxToGlobalTR(const PxRigidActor& body);
	
	// elimina memoria
	void releaseScene();
	void releaseBody(PxActor& body);

	// MAIN SINGLETON
	void init();
	void update(bool interactive, double t);
	void close(bool interactive);

	// utils
	void stepPhysics(bool interactive, double t);
	void onCollision(physx::PxActor* actor1, physx::PxActor* actor2);
	void onTrigger(physx::PxActor* actor1, physx::PxActor* actor2);

	// TIMER
	void StartCounter();
	double GetCounter();
	double GetLastTime();

	// testing debug
	void debugTime();
	void debugBall();
	void debugBuddy(Entidad* e);
	void debugBody(PxRigidDynamic* rd);
	void debugAllBodies();

	// FACTORY
	PxRigidDynamic* createDynamic(const PxTransform& transform, const PxVec3& velocity = PxVec3(PxZero));
	PxRigidDynamic* createDynamic(const PxTransform& transform, const PxGeometry& geometry, PxMaterial& material, const PxVec3& velocity = PxVec3(PxZero));
	PxRigidDynamic* createDynamic(const PxTransform& transform, PxShape* shape, const PxVec3& velocity = PxVec3(PxZero));
	
	PxRigidStatic* createStaticRigid(const PxTransform& transform);
	PxRigidStatic* createStaticRigid(const PxTransform& transform, const PxGeometry& geom, PxMaterial& material);
	PxRigidStatic* createStaticRigid(const PxTransform& transform, PxShape* shape);

	PxShape* createShape(const PxGeometry& geom, PxMaterial& material, bool isExclusive = false);
	PxShape* createTriggerShape(const PxGeometry& geom, PxMaterial& material, bool isExclusive = false);

	// factory prefabs
	PxRigidStatic* createTriggerStaticBox(const PxVec3 halfExtent = PxVec3(10.0f, 1.0f, 10.0f), const PxTransform& transform = PxTransform(0.0f, 10.0f, 0.0f));
	PxRigidDynamic* createBall();
	void createStackBoxes(const PxTransform& t, PxU32 size, PxReal halfExtent);
	void tiledStacks(PxReal num = 5, PxReal sideLength = 1.0f);

	// Getters
	int getID(int k) { return _ids[k]; };
	std::vector<int>* getIDs() { return &_ids; };
	PxPhysics* getPhysX() { return mPhysics; };
	PxScene* getScene() { return mScene; };
	PxMaterial* getMaterial() { return mMaterial; };
	Entidad* findEntityByPxActor(PxActor* actor);

	// Setters
	void addEntityID(int id) { _ids.push_back(id); };
	void addEntityToEraseID(int id) { ids_erase.push_back(id); };
	void setGlobalToPhysxTR(Entidad& e, PxRigidActor& body);
	void setPhysxToGlobalTR(Entidad& e, PxRigidActor& body);

protected:
	static PhysxManager* _singleton;

private:
	PhysxManager();

	// ON/OFF physics
	bool	mPause = false;
	bool	mOneFrame = false;

	// Timer...
	double PCFreq = 0.0;
	__int64 CounterStart = 0;
	__int64 CounterLast = 0;
	__int64 GlobalTimer = 0;

	// Variables editables
	

	PxTolerancesScale scale;
	PxCudaContextManagerDesc cudaDesc;

	PxReal stackZ = 10.0f;

	// Inevitables para que funcione
	PxDefaultAllocator		mAllocator;
	PxDefaultErrorCallback	mErrorCallback;
	ContactReportCallback	mContactReportCallback;

	PxFoundation* mFoundation = NULL;
	PxPhysics* mPhysics = NULL;

	PxDefaultCpuDispatcher* mDispatcher = NULL;
	PxScene* mScene = NULL;

	PxMaterial* mMaterial = NULL;	// default material

	PxPvd* mPvd = NULL;
	PxCooking* mCooking = NULL;

	PxCudaContextManager* mCuda = NULL;

	// vector para identificar entidades
	std::vector<int> _ids;
	std::vector<int> ids_erase;
};

// Esta macro define una forma compacta para usar el Singleton PhysxManager, 
// en lugar de escribir 'PhysxManager::instance()->method()' escribiremos 'im().method()'
inline PhysxManager& pm() {
	return *PhysxManager::GetInstance();
}

 // Forma breve de acceder al creador y padre todopoderoso de las fisicas
inline PxPhysics* physX() {
	return PhysxManager::GetInstance()->getPhysX();
}
