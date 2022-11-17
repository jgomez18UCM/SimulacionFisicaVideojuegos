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

WindForceGenerator::WindForceGenerator(Vector3& v, float k1, float k2) : _windVel(v), _k1(k1), _k2(k2)
{
}

void WindForceGenerator::updateForce(Particle* particle, double duration)
{
	if (particle->getInvMass() <= 0.0f) return;

	Vector3 pVel = particle->getVel();
	Vector3 diffVel = _windVel - pVel;

	Vector3 force = _k1 * diffVel + _k2 * diffVel.magnitude() * diffVel;
	particle->addForce(force);
}

void DragForceGenerator::updateForce(Particle* particle, double duration)
{
	if (particle->getInvMass() <= 0.0f) return;

	Vector3 force = k * particle->getVel();
	particle->addForce(force);
}

void TwisterForceGenerator::updateForce(Particle* particle, double duration)
{
	Vector3 pos = particle->getPos();
	_windVel = _Kt *  Vector3(-(pos.z - _center.z) - 0.2, 10 - (pos.y - _center.y), pos.x - _center.x - 0.2);
	WindForceGenerator::updateForce(particle, duration);
}
