#pragma once
#include "RigidBody.h"
#include "core.hpp"
#include <list>
#include <string>
#include <random>

class RigidBodyGenerator
{
public:
	struct Config {
		Vector3 _mean_pos = {0, 0, 0};
		Vector3 _mean_vel = {0, 0 ,0};
		Vector4 _color = {1, 1, 1, 1};
		double _generation_time = 2;
		double _rigid_lifetime = 10;
		std::string _name = "";
		physx::PxShape* _shape = CreateShape(physx::PxSphereGeometry(1));
	};
	virtual RigidBody* generateParticles(double t) = 0;
	virtual void init(Config& c);
	std::string getName() const { return _name; }
	void setName(std::string name) { _name = name; }
	void setShape(physx::PxShape* newShape) {
		if (_shape != nullptr) _shape->release();
		_shape = newShape;
	};
	void setActive(bool ac) { active = ac; }
	virtual ~RigidBodyGenerator() {};

protected:
	Vector3 _mean_pos, _mean_vel;
	Vector4 _modelColor;
	double _generation_time;
	double _timer;
	double _rigid_lifetime;
	std::string _name;
	physx::PxShape* _shape;
	bool active = true;
	physx::PxScene* gScene;
	physx::PxPhysics* gPhysics;
};

class GaussianRBG : public RigidBodyGenerator {

public:
	struct Config : RigidBodyGenerator::Config {
		Vector3 _dev_pos = {0, 0, 0};
		Vector3 _dev_vel = {0, 0, 0};
	};

	GaussianRBG(Config& c, physx::PxScene* _scene, physx::PxPhysics* _physics);
	~GaussianRBG() {};
	RigidBody* generateParticles(double t) override;
	void init(Config& c);

protected:
	Vector3 _dev_pos, _dev_vel;
	std::normal_distribution<float> dist;
	std::mt19937 random_generator;
};
