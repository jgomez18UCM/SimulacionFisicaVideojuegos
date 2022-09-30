#pragma once

#include "core.hpp"
#include "RenderUtils.hpp"

class Particle
{
public:
	Particle(Vector3 Pos, Vector3 Vel, Vector3 Acc, float damp);
	virtual ~Particle();

	void integrate(double t);
protected:
	Vector3 vel, acc;
	float damping;
	physx::PxTransform pose;
	RenderItem* renderItem;
};

