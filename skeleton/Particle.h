#pragma once

#include "core.hpp"
#include "RenderUtils.hpp"

class Particle
{
public:
	Particle(Vector3 Pos, Vector3 Vel, Vector3 Acc, float damp, Vector4 color = {1,1,1,1}, physx::PxShape* sh = CreateShape(physx::PxSphereGeometry(1)));
	
	virtual ~Particle();

	virtual void integrate(double t);
	virtual bool isAlive() { return timeLimit > 0; }
	virtual Particle* clone() {
		return new Particle(pose.p, vel, acc, damping, renderItem->color, renderItem->shape);
	};
	virtual void setVel(Vector3 vel) { this->vel = vel; };
	virtual void setPos(Vector3 pos) { this->pose.p = pos; };

	virtual Vector3 getPos() { return this->pose.p; };
	virtual Vector3 getVel() { return this->vel; };
protected:
	Vector3 vel, acc;
	float damping;
	physx::PxTransform pose;
	RenderItem* renderItem;
	double timeLimit;
};

class Floor : public Particle {
public:
	Floor(Vector3 Pos, double dimX, double dimY) : Particle(Pos,Vector3(0,0,0),Vector3(0,0,0), 0.0f, { 0.9,0.5,0.9,1 }, CreateShape(physx::PxBoxGeometry(dimY, 0.1, dimX))) {

	}
	virtual ~Floor() {};
};

