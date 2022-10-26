#pragma once
#include "Particle.h"
#include "UniformParticleGenerator.h"
#include "GaussianParticleGenerator.h"
#include "SphereParticleGenerator.h"
#include "Firework.h"
#include <memory>
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
				Firework* f = dynamic_cast<Firework*>(*it);
				if (f != nullptr) {
					for (auto i : f->explode() ){
						_particles.push_back(i);
					}
				}
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
	void generateFireworksSystem() { 
		Particle* i = new Particle({ 0,0,0 }, { 0,0,0 }, { 0,0,0 }, 0.99, 2);

		std::shared_ptr<SphereParticleGenerator> p;
		p.reset(new SphereParticleGenerator(20, { 0,30,0 }, i, 20));
		Firework* f = new Firework(pos, { 0,10,0 }, { 0,0,0 }, 0.99, 3, { 1,0,0,1 }, CreateShape(physx::PxSphereGeometry(1)), {p});
		_particles.push_back(f);


	};
	void generateFogSystem() {
		Particle* p = new Particle(pos, { 0,0,0 }, { 0,0,0 }, 0.99, 5, { .5,.5,1,1 }, CreateShape(physx::PxSphereGeometry(.5)));
		_generators.push_back(new SphereParticleGenerator(20,{0,0,0},p,20));
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