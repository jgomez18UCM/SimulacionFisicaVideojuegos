#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"
#include <list>
#include "RigidBody.h"
#include "RigidBodyGenerator.h"
#include "RigidBodyFG.h"
#include "RBForceRegistry.h"

class WorldManager
{
public:
	WorldManager(physx::PxScene* scene, physx::PxPhysics* physics);
	void update(double t);
	void generatorDemo();
	~WorldManager();
protected:
	physx::PxScene* gScene;
	physx::PxPhysics* gPhysics;
	std::list<RigidBody*> _rigids;
	std::list<RigidBodyFG*> _forceGens;
	std::list<RigidBodyGenerator*> _RBGens;
	RBForceRegistry _forceRegs;
	RigidBodyGenerator* gen = nullptr;
	physx::PxRigidStatic* suelo;
	RenderItem* sueloRender;
};

