#include "Particle.h"

Particle::Particle(Vector3 Pos, Vector3 Vel, Vector3 Acc, float damp, float time, Vector4 color, physx::PxShape* sh ) {
	this->vel = Vel;
	this->pose = physx::PxTransform(Pos);
	this->acc = Acc;
	this->damping = damp;
	this->renderItem = new RenderItem(sh, &this->pose, color);
	this->timeLimit = time;
}

Particle::~Particle() {
	DeregisterRenderItem(this->renderItem);
}

void Particle::integrate(double t) {
	this->pose.p += this->vel * t ;
	this->vel = this->vel * pow(this->damping, t) + this->acc * t ;
	this->timeLimit -= t;
}