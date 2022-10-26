#pragma once
#include "ParticleGenerator.h"
#include <random>
class SphereParticleGenerator : public ParticleGenerator 
{
public:
	SphereParticleGenerator(int numParticles, Vector3 pos, Particle* model, float radius);
	virtual ~SphereParticleGenerator() {};
	std::list<Particle*> generateParticles() override;

private:
	float _radius;
	std::uniform_real_distribution<float> random;
};