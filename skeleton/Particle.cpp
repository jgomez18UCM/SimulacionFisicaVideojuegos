#include "Particle.h"

Particle::Particle(Vector3 Pos, Vector3 Vel, Vector3 Acc, float damp, float mass, float time, Vector4 color, physx::PxShape* sh, bool render) {
	this->vel = Vel;
	this->pose = physx::PxTransform(Pos);
	this->acc = Acc;
	this->damping = damp;
	this->color = color;
	this->shape = sh;
	if (render) {
		this->renderItem = new RenderItem(sh, &this->pose, color);
	}
	else {
		this->renderItem = nullptr;
	}
	this->timeLimit = time;
	this->force = { 0,0,0 };
	mass > 0.0f ? this->inverse_mass = 1.0f/mass : this->inverse_mass = 0.0f;
	this->mass = mass;
}

Particle::~Particle() {
	DeregisterRenderItem(this->renderItem);
}

void Particle::integrate(double t) {
	if (inverse_mass <= 1e-96f) return;
	this->pose.p += this->vel * t;
	auto momentAcc = acc + force * inverse_mass;
	this->vel = this->vel * pow(this->damping, t) + momentAcc * t ;
	this->timeLimit -= t;
	clearForce();
}