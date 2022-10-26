#pragma once
#include "Particle.h"
#include "UniformParticleGenerator.h"
#include "GaussianParticleGenerator.h"
class ParticleSystem{
protected:
	std::list<Particle*> _particles;
	std::list<ParticleGenerator*> _generators;
	Vector3 pos;
public:
	ParticleSystem(Vector3 Pos)  {
		pos = Pos;
	};
	void update(double t) {
		for (auto g : _generators)
		{
			for (auto p : g->generateParticles()) {
				_particles.push_back(p);
			}
		}

		for (auto it = _particles.begin(); it != _particles.end();) {
			(*it)->integrate(t);

			if (!(*it)->isAlive()) {
				delete (*it);
				it = _particles.erase(it);
			}
			else {
				it++;
			}
		}

	};
	ParticleGenerator* getParticleGenerator(std::string name) {
		for (auto g : _generators) {
			if (g->getName() == name)
				return g;
		}
	};
	void generateFireworksSystem() {  };
	void generateFogSystem() {
		_generators.push_back(new GaussianParticleGenerator("Fog", CreateShape(physx::PxSphereGeometry(.5)), {.5,.5,1,1},
			pos, { 0,0,0 }, { 0,0,0 }, 75, { 0.1,10,10 }, { 0.1,0.1,0.1 }, 3));
	};
	virtual ~ParticleSystem() {
		for (auto it = _particles.begin(); it != _particles.end(); it = _particles.erase(it)) {
			delete (*it);
		};
		for (auto gen = _generators.begin(); gen != _generators.end(); gen = _generators.erase(gen) ) {
			delete (*gen);
		}
			
	};
};