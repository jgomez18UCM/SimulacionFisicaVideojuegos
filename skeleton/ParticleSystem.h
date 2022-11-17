#pragma once
#include "Particle.h"
#include "UniformParticleGenerator.h"
#include "GaussianParticleGenerator.h"
#include "SphereParticleGenerator.h"
#include "Firework.h"
#include "ForceGenerator.h"
#include "ParticleForceRegistry.h"
#include <memory>
class ParticleSystem{
protected:
	std::list<Particle*> _particles;
	std::list<ParticleGenerator*> _generators;
	ParticleForceRegistry forces;
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

		forces.updateForces(t);

		for (auto it = _particles.begin(); it != _particles.end();) {
			(*it)->integrate(t);

			if (!(*it)->isAlive()) {
				Firework* f = dynamic_cast<Firework*>(*it);
				if (f != nullptr) {
					for (auto i : f->explode() ){
						_particles.push_back(i);
					}
				}
				forces.deleteParticleregistry(*it);
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
		Particle* i = new Particle({ 0,0,0 }, { 0,0,0 }, { 0,0,0 }, 0.99, 2.0f, 2, {0,1,0,1}, CreateShape(physx::PxSphereGeometry(1)), false);

		std::shared_ptr<SphereParticleGenerator> p;
		p.reset(new SphereParticleGenerator(20, { 0,30,0 }, i, 20));
		Firework* f = new Firework(pos, { 0,20,0 }, { 0,0,0 }, 0.99, 3.0f, 1.5, { 1,0,0,1 }, CreateShape(physx::PxSphereGeometry(3)), {p});
		_particles.push_back(f);


	};
	void generateFogSystem() {
		if (getParticleGenerator("Fog") == nullptr) {
			Particle* p = new Particle(pos, { 0,0,0 }, { 0,0,0 }, 0.99, 0.0f, 5, { .7,.7,.7,1 }, CreateShape(physx::PxSphereGeometry(.5)), false);
			_generators.push_back(new GaussianParticleGenerator("Fog", p, 0.75, { 10,10,10 }, { 1,1,1 }, 1000));
		}
	};
	void generateGravityDemo() {
		Particle* part = new Particle({ 10,10,10 }, { 0,0,0 }, { 0,0,0 },
			0.99, 10, 10, { 1,0,0,1 }, CreateShape(physx::PxSphereGeometry(10)), true);
		GravityForceGenerator* gen = new GravityForceGenerator(Vector3(0,-9.8,0), 100);
		forces.addRegistry(gen, part);
		_particles.push_back(part);
	}
	void generateWindDemo() {
		Particle* part = new Particle({ 10,10,10 }, { 0,0,0 }, { 0,0,0 },
			0.99, 10, 100000, { 1,0,0,1 }, CreateShape(physx::PxSphereGeometry(10)), true);
		WindForceGenerator* gen = new WindForceGenerator(Vector3(5, 0, 0), 1,0.1);
		forces.addRegistry(gen, part);
		_particles.push_back(part);
	}
	void generateTwisterDemo() {
		Particle* part = new Particle({ 10,0,10 }, { 0,0,0 }, { 0,0,0 },
			0.99, 10, 100000, { 1,0,0,1 }, CreateShape(physx::PxSphereGeometry(10)), true);
		Particle* part2 = new Particle({ 0,0,0 }, { 0,0,0 }, { 0,0,0 },
			0.99, 10, 100000, { 1,0,0,1 }, CreateShape(physx::PxBoxGeometry(1,1,1)), true);
		TwisterForceGenerator* gen = new TwisterForceGenerator(Vector3(0, 0, 0), 1, 0.1, 0.35);
		forces.addRegistry(gen, part);
		_particles.push_back(part);
	}
	virtual ~ParticleSystem() {
		for (auto it = _particles.begin(); it != _particles.end(); it = _particles.erase(it)) {
			delete (*it);
		};
		for (auto gen = _generators.begin(); gen != _generators.end(); gen = _generators.erase(gen) ) {
			delete (*gen);
		}
			
	};
};