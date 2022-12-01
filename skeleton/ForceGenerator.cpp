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
	Vector3 v = particle->getVel();
	float drag_coef = v.normalize();
	drag_coef *= k;
	Vector3 force = -v * drag_coef;
	particle->addForce(force);
}

void TwisterForceGenerator::updateForce(Particle* particle, double duration)
{
	if (particle->getInvMass() <= 0.0f) return;
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

SpringForceGenerator::SpringForceGenerator(Particle* other, float k, float resting_length) : _other(other), _k(k), _resting_length(resting_length)
{
}

void SpringForceGenerator::updateForce(Particle* p, double duration) 
{
	if (fabs(p->getInvMass()) <= 1e-10) return;

	Vector3 force = _other->getPos() - p->getPos();
	
	const float length = force.normalize();
	const float delta_x = length - _resting_length;

	force *= delta_x * _k;

	p->addForce(force);
}

AnchoredSpringFG::AnchoredSpringFG(Vector3& position, float k, float resting_length) :
	SpringForceGenerator(nullptr, k, resting_length)
{
	_other = new Particle(position, { 0,0,0 }, { 0,0,0 }, 0, 0, 0, { 1,0,0,1 }, CreateShape(physx::PxBoxGeometry(1, 1, 1)));
}

AnchoredSpringFG::~AnchoredSpringFG()
{
	delete _other;
}

BungeeForceGenerator::BungeeForceGenerator(Particle* other, float k, float resting_length) :
	SpringForceGenerator(other,k,resting_length){}

void BungeeForceGenerator::updateForce(Particle* p, double duration)
{
	if (fabs(p->getInvMass()) <= 1e-10) return;

	Vector3 force = _other->getPos() - p->getPos();

	const float length = force.normalize();
	const float delta_x = length - _resting_length;

	if (delta_x <= 0.0f) return;

	force *= delta_x * _k;

	p->addForce(force);
}

BuoyancyForceGenerator::BuoyancyForceGenerator(float height, float V, float d, Particle* liquid_surface) :
	_height(height), _volume(V), _density(d), _liquid_particle(liquid_surface)
{
}

void BuoyancyForceGenerator::updateForce(Particle* p, double duration)
{
	if (fabs(p->getInvMass()) <= 1e-10) return;

	float h = p->getPos().y;
	float h0 = _liquid_particle->getPos().y;

	Vector3 f(0,0,0);
	float immersed = 0.0;
	if (h0 - h > _height * 0.5) {
		immersed = 1.0;
	}
	else if (h - h0 > _height * 0.5) {
		immersed = 0.0;
	}
	else {
		immersed = (h0 - h) / _height + 0.5;
	}

	f.y = _density * _volume * immersed * 9.8;
	p->addForce(f);
}
