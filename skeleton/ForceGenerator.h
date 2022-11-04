#pragma once
#include "Particle.h"
#include <list>
#include <string>

class ForceGenerator
{
public:
	virtual void updateForce(Particle* particle, double duration) = 0;
	std::string name;
	double t = -1;
};

class GravityForceGenerator : public ForceGenerator {
public:
	GravityForceGenerator(Vector3 & G, double t);
	~GravityForceGenerator() {};
	void updateForce(Particle* particle, double duration) override;
protected:
	Vector3 g;
};

