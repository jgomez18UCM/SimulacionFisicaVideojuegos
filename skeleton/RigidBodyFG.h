#pragma once
#include <string>
#include "core.hpp"
#include "RigidBody.h"

class RigidBodyFG
{
public:
	virtual void updateForce(RigidBody* rb, double t) = 0;
	std::string name;
	double t = -1;
};

class RigidBodyWindFG : public RigidBodyFG {
public:
	RigidBodyWindFG(Vector3 windVel, float k1, float k2);
	void updateForce(RigidBody* rb, double t) override;
protected:
	float _k1, _k2;
	Vector3 _windVel;
};