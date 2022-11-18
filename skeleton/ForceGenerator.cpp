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
	Vector3 dist = pos - _center;
	_windVel = _Kt *  Vector3(-(dist.z) - (dist.x), 50 - (dist.y), dist.x - (dist.z));
	WindForceGenerator::updateForce(particle, duration);
}

void ExplosionForceGenerator::updateForce(Particle* p, double duration)
{
	if (p->getInvMass() <= 1e-10f) return;
	Vector3 distV =   p->getPos() - _center;
	
	float dist = distV.normalize();
	float r = dist*dist;
	if (dist == 0) dist = 1e-5;
	Vector3 force = (_k / r) * distV *exp(-t/_kt);
	if(dist <= _radius) p->addForce(force);
}
