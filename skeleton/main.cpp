#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>

#include "Particle.h"
#include "Proyectile.h"
#include "WorldManager.h"
#include "ParticleSystem.h"

#include "RigidBody.h"


using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;


PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene      = NULL;
ContactReportCallback   gContactReportCallback;
Particle*				suelo;
Projectile*				proj;
std::vector<Projectile*> projs;
ParticleSystem* sys;
RigidBody* rg;
WorldManager* wm;
// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(),true,gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);
	//suelo = new Floor(Vector3(0, -10, 0), 1000, 1000);
	sys = new ParticleSystem({ 0,0,0 });
	//sys->generateBuoyancyDemo();

	/*PxRigidDynamic* rigid;
	rigid = gPhysics->createRigidDynamic(physx::PxTransform({ 0,0,0 }));
	rigid->setMass(10);
	PxShape* shape = CreateShape(PxBoxGeometry(Vector3(.5,.5,.5)));
	rigid->attachShape(*shape);
	physx::PxRigidBodyExt::updateMassAndInertia(*rigid, 1);
	gScene->addActor(*rigid);*/
	//rg = new RigidBody(rigid, 10, { 1,1,1,1 });
	wm = new WorldManager(gScene, gPhysics);
	 
	//proj = new RPGproyecyile(GetCamera()->getEye(), GetCamera()->getDir());
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);
	wm->update(t);
	for (auto it = projs.begin(); it != projs.end();) {
		(*it)->integrate(t);
		if (!(*it)->isAlive()) {
			delete (*it);
			it = projs.erase(it);
		}
		else {
			++it;
		}
	}
	sys->update(t);
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	delete rg;
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();	
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
	
	gFoundation->release();

	for (auto it = projs.begin(); it != projs.end();) {
		delete (*it);
		it = projs.erase(it);
	}
	delete sys;
	
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);
	wm->getPlayer()->handleKey(toupper(key));
	switch(toupper(key))
	{
	//case 'B': break;
	//case ' ':	break;
	case '1':
	{
		wm->Wind();
		break;
	}
	case '2':
		wm->Twister();
		break;
	case '3':
		sys->generateSpringDemo();
		break;
	case '4':
		sys->generateFireworksSystem();
		break;
	default:
		break;
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}


int main(int, const char*const*)
{
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for(PxU32 i=0; i<frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}