#include "Particle.h"

Particle::Particle(Vector3 Pos, Vector3 Vel, Vector3 Acc, float damp) {
	this->vel = Vel;
	this->pose = physx::PxTransform(Pos);
	this->acc = Acc;
	this->damping = damp;
	this->renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(1)), &this->pose, Vector4(1,0,0,1));
}

Particle::~Particle() {
	DeregisterRenderItem(this->renderItem);
}

void Particle::integrate(double t) {
	this->pose.p += this->vel * t ;
	this->vel = this->vel * pow(this->damping, t) + this->acc * t ;
}