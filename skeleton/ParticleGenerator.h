#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"
#include <list>
#include <string>
#include "Particle.h"

class ParticleGenerator {
public:
	virtual std::list<Particle*> generateParticles() = 0;
	std::string getName() const { return _name; }
	void setName(std::string name) { _name = name; }
	void setParticle(Particle* part) { _model = part; }
	virtual ~ParticleGenerator() = 0;

protected:
	Vector3 _mean_pos, _mean_vel;
	double _generation_probability;
	Particle* _model;
	int _num_particles;
	std::string _name;
};