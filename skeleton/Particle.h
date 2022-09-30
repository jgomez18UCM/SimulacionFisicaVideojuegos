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

class Floor : public Particle {
public:
	Floor(Vector3 Pos, double dimX, double dimY) : Particle(Pos,Vector3(0,0,0),Vector3(0,0,0), 0.0f) {
		DeregisterRenderItem(renderItem);
		renderItem = new RenderItem(CreateShape(physx::PxBoxGeometry(dimY, 0.1, dimX)), &pose, { 0.9,0.5,0.9,1 });
	}
	virtual ~Floor() {};
};

