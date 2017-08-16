#include "PhysicsSystem.hpp"
#include "Transform.hpp"
#include "Debug.hpp"
//#include <SnippetCommon/SnippetPrint.h>
//#include <SnippetCommon/SnippetPVD.h>
//#include <SnippetUtils/SnippetUtils.h>
#include <pvd/PxPvd.h>

using namespace FishEngine;
using namespace physx;

class FishEnginePhysxErrorCallback : public PxErrorCallback
{
public:
	virtual void reportError(PxErrorCode::Enum code,
							 const char* message,
							 const char* file,
							 int line) override
	{
		//if (code & PxErrorCode::eDEBUG_INFO != 0)
		LogError(Format( "PyhsX Error[file %s at line %d]: %s", file, line, message ));
		//abort();
	}
};

//class FishEnginePhysxAllocator : public PxAllocatorCallback
//{
//public:
//    void* allocate(size_t size, const char*, const char*, int)
//    {
//        void* ptr = ::memalign(16, size);
//        PX_ASSERT((reinterpret_cast<size_t>(ptr) & 15)==0);
//        return ptr;
//    }
//    
//    void deallocate(void* ptr)
//    {
//        ::free(ptr);
//    }
//};

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene		= NULL;

PxMaterial*				gMaterial	= NULL;

//PxVisualDebuggerConnection* gConnection	= NULL;

PxPvd * gPvd = NULL;

#define PVD_HOST "localhost"

void FishEngine::PhysicsSystem::Init()
{
	static PxDefaultAllocator		gAllocator;
	//FishEnginePhysxAllocator		gAllocator;
	static PxDefaultErrorCallback	gErrorCallback;
	//static FishEnginePhysxErrorCallback       gErrorCallback;
	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);
	if (gFoundation == nullptr) {
		LogError("[PhysX] create foundation failed");
		abort();
	}
	// PxProfileZoneManager* profileZoneManager = &PxProfileZoneManager::createProfileZoneManager(gFoundation);
	
//	gPvd = PxCreatePvd(*gFoundation);
//	auto transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
//	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
	
	bool recordMemoryAllocations = true;
	PxTolerancesScale scale;
	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, scale, recordMemoryAllocations, nullptr);
	
//	if(gPhysics->getPvdConnectionManager())
//	{
//		gPhysics->getVisualDebugger()->setVisualizeConstraints(true);
//		gPhysics->getVisualDebugger()->setVisualDebuggerFlag(PxVisualDebuggerFlag::eTRANSMIT_CONTACTS, true);
//		gPhysics->getVisualDebugger()->setVisualDebuggerFlag(PxVisualDebuggerFlag::eTRANSMIT_SCENEQUERIES, true);
//		gConnection = PxVisualDebuggerExt::createConnection(gPhysics->getPvdConnectionManager(), PVD_HOST, 5425, 10);
//	}
	
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher	= gDispatcher;
	sceneDesc.filterShader	= PxDefaultSimulationFilterShader;
	//sceneDesc.flags |= physx::PxSceneFlag::eENABLE_ACTIVETRANSFORMS;
	gScene = gPhysics->createScene(sceneDesc);
	
	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
}

void FishEngine::PhysicsSystem::FixedUpdate()
{
	gScene->simulate(1.0f/30.f);
	gScene->fetchResults(true);
}

void FishEngine::PhysicsSystem::Clean()
{
	gScene->release();
	gDispatcher->release();
	//PxProfileZoneManager* profileZoneManager = gPhysics->getProfileZoneManager();
//	if(gConnection != NULL)
//		gConnection->release();
	gPhysics->release();
	if (gPvd != nullptr)
		gPvd->release();
//	profileZoneManager->release();
	gFoundation->release();
	
	LogInfo("Clean up PhysX.");
}
