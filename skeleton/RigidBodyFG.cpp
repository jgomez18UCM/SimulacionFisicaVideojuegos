#include "RigidBodyFG.h"

RigidBodyWindFG::RigidBodyWindFG(Vector3 windVel, float k1, float k2) : _k1(k1), _k2(k2), _windVel(windVel)
{
}

void RigidBodyWindFG::updateForce(RigidBody* rb, double t)
{
	if (rb->getInvMass() <= 0) return;

	Vector3 pVel = rb->getLinearVelocity();
	Vector3 diffVel = _windVel - pVel;

	Vector3 force = _k1 * diffVel + _k2 * diffVel.magnitude() * diffVel;
	rb->addForce(force);
}
