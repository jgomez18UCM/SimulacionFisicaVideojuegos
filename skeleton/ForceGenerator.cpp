#include "ForceGenerator.h"

GravityForceGenerator::GravityForceGenerator(Vector3 & G, double t) {
	g = G;
	this->t = t;
}

void GravityForceGenerator::updateForce(Particle* particula, double duration) {
	if (particula->getInvMass() <= 0.0f)
		return;

	particula->addForce(g * particula->getMass());
}