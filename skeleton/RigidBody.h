#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"

class RigidBody
{
public:
	enum Shape {
		Box,
		Sphere,
		Capsule
	};
	RigidBody(physx::PxRigidDynamic* rigid, double time, Vector4 color);
	RigidBody(physx::PxScene* scene, physx::PxPhysics*, Vector3 pos, Vector3 vel, double mass, double time, Vector3 dims, Shape s, Vector4 color);
	virtual ~RigidBody();
	void update(double t);
	bool isAlive();

	void addForce(Vector3 f);
	void addTorque(Vector3 f);

	Vector3 getPosition() const;
	Vector3 getLinearVelocity() const;
	float getMass() const;
	float getInvMass() const;

	void setLinearVelocity(Vector3& vel);
	void unrender();
	void setMass(float mass) { _rigid->setMass(mass); };

protected:
	physx::PxRigidDynamic* _rigid;
	RenderItem* _render_item;
	double _lifeTime;
	double _life;
};

