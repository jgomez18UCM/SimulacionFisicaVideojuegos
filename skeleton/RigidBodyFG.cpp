#include "RigidBodyFG.h"

bool RigidBodyFG::checkRes(Vector3& pos) {
	return pos.x > _res.maxX || pos.x < _res.minX || pos.y > _res.maxY || pos.y < _res.minY || pos.z > _res.maxZ || pos.z < _res.minZ;
}

RigidBodyWindFG::RigidBodyWindFG(Vector3 windVel, float k1, float k2) : _k1(k1), _k2(k2), _windVel(windVel)
{
}

void RigidBodyWindFG::updateForce(RigidBody* rb, double t)
{
	if (rb->getInvMass() <= 0) return;
	if (!_res.defecto) {
		auto pos = rb->getPosition();
		if (checkRes(pos)) return;
	}

	Vector3 pVel = rb->getLinearVelocity();
	Vector3 diffVel = _windVel - pVel;

	Vector3 force = _k1 * diffVel + _k2 * diffVel.magnitude() * diffVel;
	rb->addForce(force);
}

void RBTwisterFG::updateForce(RigidBody* rb, double duration)
{
	if (rb->getInvMass() <= 0.0f) return;
	Vector3 pos = rb->getPosition();
	if (!_res.defecto && checkRes(pos)) return;
	Vector3 dist = pos - _center;
	_windVel = _Kt * Vector3(-(dist.z) - (dist.x), 20 - (dist.y), dist.x - (dist.z));
	RigidBodyWindFG::updateForce(rb, duration);
}
