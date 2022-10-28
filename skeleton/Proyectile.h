#pragma once
#include "Particle.h"
class Projectile : public Particle
{
public:
	enum Types {
		RPG,
		Pistol
	};
	Projectile(Vector3 Pos, Vector3 dir, Types type) : Particle(Pos, dir, Vector3(0, 0, 0), 0.99, 2.0f, 5,{ 1,1,1,1 },CreateShape(physx::PxSphereGeometry(1))) {

		DeregisterRenderItem(this->renderItem);
		auto cameraQ = GetCamera()->getTransform().q;
		switch (type) {
			case RPG:
				this->pose.q = {cameraQ.x, cameraQ.z, cameraQ.y, cameraQ.w};
				this->vel *= 10;
				this->renderItem = new RenderItem(CreateShape(physx::PxCapsuleGeometry(3, 5)), &this->pose, Vector4(.9, .9, .9, 1));
				this->inverse_mass = 1/10.0f;
				break;
			case Pistol:
				vel = 300 * dir;
				acc = Vector3(0, -9.8, 0);
				damping = 0.9;
				this->renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(3)), &this->pose, Vector4(.9, .9, .9, 1));
				break;
			default:
				break;
		}
	}

	virtual ~Projectile(){};
};

