#pragma once
#include "ParticleGenerator.h"
class SimpleParticleSystem : public ParticleGenerator
{
public:
	SimpleParticleSystem(std::string Name) {};
	virtual std::list<Particle*> generateParticles() {};
protected:
	int _num_particles;
	std::list<Particle*> _particles;
};

