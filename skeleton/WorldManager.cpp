#include "WorldManager.h"

WorldManager::WorldManager(physx::PxScene* scene, physx::PxPhysics* physics)
{
	gScene = scene;
	gPhysics = physics;

	suelo = gPhysics->createRigidStatic(physx::PxTransform({0, 0, 0}));
	physx::PxShape* shape = CreateShape(physx::PxBoxGeometry(Vector3(100, .5, 100)));
	suelo->attachShape(*shape);
	scene->addActor(*suelo);
	sueloRender = new RenderItem(shape, suelo, {0, 1, .2, 1});


	createPlayer();
	for (int i = 0; i < 3; i++) {
		generateRoom(Vector3(20, 0, i * 20));
	}
}

WorldManager::~WorldManager()
{
	for (auto it = _forceGens.begin(); it != _forceGens.end(); it = _forceGens.erase(it)) {
		delete* it;
	}
	for (auto it = _RBGens.begin(); it != _RBGens.end(); it = _RBGens.erase(it)) {
		delete* it;
	}
	for (auto it = _rigids.begin(); it != _rigids.end(); it = _rigids.erase(it)) {
		delete* it;
	}
	for (auto it = _staticRenderers.begin(); it != _staticRenderers.end(); it = _staticRenderers.erase(it)) {
		if (*it != nullptr) DeregisterRenderItem(*it);
	}
	delete player;
}

void WorldManager::createPlayer()
{
	physx::PxRigidDynamic* playerRD = gPhysics->createRigidDynamic(physx::PxTransform({ 0, 5, 0 }));
	physx::PxShape* Pshape = CreateShape(physx::PxBoxGeometry(Vector3(3, 3, 3)));
	physx::PxMaterial* mat = gPhysics->createMaterial(0, 0, 0);
	Pshape->setMaterials(&mat, 1);
	playerRD->attachShape(*Pshape);
	gScene->addActor(*playerRD);
	physx::PxRigidBodyExt::updateMassAndInertia(*playerRD, 1);
	RigidBody* playerRB = new RigidBody(playerRD, -1, { 0,0,0,1 });
	_rigids.push_back(playerRB);
	player = new Player(playerRB, GetCamera());
}

void WorldManager::generateRoom(Vector3 pos) {
	physx::PxRigidStatic* pared1 = gPhysics->createRigidStatic(physx::PxTransform({ pos.x, 10, pos.z + 10 }));
	physx::PxShape* shapeP1 = CreateShape(physx::PxBoxGeometry(Vector3(10, 10, 1)));
	pared1->attachShape(*shapeP1);
	gScene->addActor(*pared1);
	RenderItem* pared1Render = new RenderItem(shapeP1, pared1, { .2, 0, 1, 1 });
	_staticRenderers.push_back(pared1Render);

	physx::PxRigidStatic* pared2 = gPhysics->createRigidStatic(physx::PxTransform({ pos.x, 10, pos.z - 10}));
	physx::PxShape* shapeP2 = CreateShape(physx::PxBoxGeometry(Vector3(10, 10, 1)));
	pared2->attachShape(*shapeP2);
	gScene->addActor(*pared2);
	RenderItem* pared2Render = new RenderItem(shapeP2, pared2, { .2, 0, 1, 1 });
	_staticRenderers.push_back(pared2Render);

	physx::PxRigidStatic* paredAtras = gPhysics->createRigidStatic(physx::PxTransform({ pos.x-10, 10, pos.z }));
	physx::PxShape* shapeAtras = CreateShape(physx::PxBoxGeometry(Vector3(1, 10, 10)));
	paredAtras->attachShape(*shapeAtras);
	gScene->addActor(*paredAtras);
	

	physx::PxRigidStatic* paredAlante = gPhysics->createRigidStatic(physx::PxTransform({ pos.x+10, 10, pos.z }));
	physx::PxShape* shapeAlante = CreateShape(physx::PxBoxGeometry(Vector3(1, 10, 10)));
	paredAlante->attachShape(*shapeAlante);
	gScene->addActor(*paredAlante);
	RenderItem* paredAlanteRender = new RenderItem(shapeAlante, paredAlante, { .2, 0, 1, 1 });
	_staticRenderers.push_back(paredAlanteRender);

	physx::PxRigidStatic* techo = gPhysics->createRigidStatic(physx::PxTransform({ pos.x, 20, pos.z}));
	physx::PxShape* shapeTecho = CreateShape(physx::PxBoxGeometry(Vector3(10, 1, 10)));
	techo->attachShape(*shapeTecho);
	gScene->addActor(*techo);
	RenderItem* renderTecho = new RenderItem(shapeTecho, techo, { .2, 0, 1, 1 });
	_staticRenderers.push_back(renderTecho);
}

void WorldManager::update(double t) {
	for (auto gen : _RBGens) {
		RigidBody* rb = gen->generateParticles(t);
		if(rb!=nullptr)
		{
			_rigids.push_back(rb);
			for (auto force : _forceGens) {
				_forceRegs.addRegistry(force, rb);
			}
		}
	}
	_forceRegs.updateForces(t);
	for (auto it = _rigids.begin(); it != _rigids.end(); ) {
		(*it)->update(t);
		if (!(*it)->isAlive()) {
			_forceRegs.deleteRigidRegistry(*it);
			delete (*it);
			it = _rigids.erase(it);
		}
		else {
			++it;
		}
	}
	player->update();
}

void WorldManager::Wind() {

	GaussianRBG::Config c;
	c._mean_pos = { 15,5,0 };
	c._dev_pos = { 0,0,3 };
	c._generation_time = .5;
	c._rigid_lifetime = 3;
	c._shape = CreateShape(physx::PxBoxGeometry(1, 1, 1));
	c._color = { 1,0,0,1 };
	GaussianRBG* s = new GaussianRBG(c, gScene, gPhysics);
	_RBGens.push_back(s);

	RigidBodyWindFG* wind = new RigidBodyWindFG({ 10,0,0 }, 1, .1);
	Restraint res;
	res.minX = 10;
	res.maxX = 30;
	res.minZ = -10;
	res.maxZ = 10;
	res.maxY = 100;
	res.minY = -100;
	res.defecto = false;
	wind->setRestraint(res);
	_forceGens.push_back(wind);
}

void WorldManager::Twister() {

	GaussianRBG::Config c;
	c._mean_pos = { 15,5,20 };
	c._dev_pos = { 0,0,3 };
	c._generation_time = .5;
	c._rigid_lifetime = 15;
	c._shape = CreateShape(physx::PxSphereGeometry(.5));
	c._color = { 1,.5,.30,1 };
	GaussianRBG* s = new GaussianRBG(c, gScene, gPhysics);
	_RBGens.push_back(s);

	RigidBodyWindFG* wind = new RBTwisterFG({ 20,0,20 }, 1, .1, .75);
	Restraint res;
	res.minX = 10;
	res.maxX = 30;
	res.minZ = 10;
	res.maxZ = 30;
	res.maxY = 100;
	res.minY = -100;
	res.defecto = false;
	wind->setRestraint(res);
	_forceGens.push_back(wind);
}