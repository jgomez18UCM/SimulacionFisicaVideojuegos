#include "Particle.h"

Particle::Particle(Vector3 Pos, Vector3 Vel) {
	this->vel = Vel;
	this->pose = physx::PxTransform(Pos);
	this->renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(1)), &this->pose, Vector4(1,0,0,1));
}

Particle::~Particle() {
	DeregisterRenderItem(this->renderItem);
}

void Particle::integrate(double t) {
	this->pose.p += this->vel * t;
}