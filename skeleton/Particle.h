#pragma once

#include "core.hpp"
#include "RenderUtils.hpp"

class Particle
{
public:
	Particle(Vector3 Pos, Vector3 Vel, Vector3 Acc, float damp, float mass, float time = 5.0f, Vector4 color = { 1,1,1,1 }, physx::PxShape* sh = CreateShape(physx::PxSphereGeometry(1)), bool render = true);
	
	virtual ~Particle();

	virtual void integrate(double t);
	virtual bool isAlive() { return timeLimit > 0; }
	virtual Particle* clone() {
		return new Particle(pose.p, vel, acc, damping, inverse_mass > 0? 1.0f/inverse_mass : 0.0f, timeLimit, color, shape);
	};
	virtual void setVel(Vector3 vel) { this->vel = vel; };
	virtual void setPos(Vector3 pos) { this->pose.p = pos; };
	virtual void setAcc(Vector3 acc) { this->acc = acc; };
	virtual void setMass(float mass) {
		if (mass <= 0) inverse_mass = 0;
		else inverse_mass = 1.0f / mass;
		this->mass = mass;
	}
	virtual void setColor(Vector4& color) {
		if(renderItem!=nullptr)DeregisterRenderItem(renderItem);
		renderItem = new RenderItem(shape, &pose, color);
	}
	virtual void clearForce() { force*=0; }
	virtual void addForce(Vector3 f) { force += f; }
	virtual Vector3 getPos() { return this->pose.p; };
	virtual Vector3 getVel() { return this->vel; };
	virtual Vector3 getAcc() { return this->acc; };
	virtual float getInvMass() { return this->inverse_mass; };
	virtual float getMass() { return mass; };
protected:
	Vector4 color;
	physx::PxShape* shape;
	Vector3 vel, acc;
	Vector3 force;
	float inverse_mass, mass;
	float damping;
	physx::PxTransform pose;
	RenderItem* renderItem;
	float timeLimit;
};

class Floor : public Particle {
public:
	Floor(Vector3 Pos, double dimX, double dimY) : Particle(Pos,Vector3(0,0,0),Vector3(0,0,0), 0.0f, 1.0f, 1, { 0.9,0.5,0.9,1 }, CreateShape(physx::PxBoxGeometry(dimY, 0.1, dimX))) {
	}
	virtual ~Floor() {};
};

