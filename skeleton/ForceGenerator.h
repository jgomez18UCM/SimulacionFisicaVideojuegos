#pragma once
#include "Particle.h"
#include <list>
#include <string>

class ForceGenerator
{
public:
	virtual void updateForce(Particle* particle, double dureation) = 0;
	std::string name;
};

