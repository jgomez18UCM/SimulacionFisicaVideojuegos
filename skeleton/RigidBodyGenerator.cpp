#include "RigidBodyGenerator.h"
#include <iostream>


void RigidBodyGenerator::init(Config& c)
{
	_mean_pos = c._mean_pos;
	_mean_vel = c._mean_vel;
	_modelColor = c._color;
	_generation_time = c._generation_time;
	_timer = _generation_time;
	_rigid_lifetime = c._rigid_lifetime;
	_name = c._name;
	_shape = c._shape;
	active = true;
}

GaussianRBG::GaussianRBG(Config& c, physx::PxScene* _scene, physx::PxPhysics* _physics)
{
	gScene = _scene;
	gPhysics = _physics;
	init(c);

	std::random_device r;
	random_generator = std::mt19937(r());
	dist = std::normal_distribution<float>(0, 1);
}

RigidBody* GaussianRBG::generateParticles(double t)
{
	_timer += t;
	if(_timer < _generation_time) return nullptr;
	_timer = 0;
	physx::PxRigidDynamic* rigid = gPhysics->createRigidDynamic({ 0,0,0 });
	rigid->attachShape(*_shape);
	gScene->addActor(*rigid);
	Vector3 pos = { _mean_pos.x + _dev_pos.x + dist(random_generator), _mean_pos.y + _dev_pos.y + dist(random_generator) ,_mean_pos.z + _dev_pos.z + dist(random_generator) };
	rigid->setGlobalPose(physx::PxTransform(pos));
	Vector3 vel = { _mean_vel.x + _dev_vel.x + dist(random_generator), _mean_vel.y + _dev_vel.y + dist(random_generator) ,_mean_vel.z + _dev_vel.z + dist(random_generator) };
	
	return new RigidBody(rigid, _rigid_lifetime, _modelColor);
}

void GaussianRBG::init(Config& c)
{
	RigidBodyGenerator::init(c);
	_dev_pos = c._dev_pos;
	_dev_vel = c._dev_vel;
}

