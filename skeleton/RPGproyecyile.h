#pragma once
#include "Particle.h"
class RPGproyecyile : public Particle
{
public:
	RPGproyecyile(Vector3 Pos, Vector3 dir) : Particle(Pos,10* dir, Vector3(0,0,0), 0.99) {
		DeregisterRenderItem(this->renderItem);
		this->pose.q = {0,.710,0,.710};
		this->renderItem = new RenderItem(CreateShape(physx::PxCapsuleGeometry(3, 5)), &this->pose, Vector4(.9, .9, .9, 1));
		
		this->mass = 10;
	}

protected:
	float mass;

};

