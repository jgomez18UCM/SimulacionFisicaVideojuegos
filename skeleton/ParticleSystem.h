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
	std::vector<ForceGenerator*> _forceGens;
	ParticleForceRegistry _forceReg;

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

		_forceReg.updateForces(t);

		for (auto it = _particles.begin(); it != _particles.end();) {
			(*it)->integrate(t);

			if (!(*it)->isAlive()) {
				Firework* f = dynamic_cast<Firework*>(*it);
				if (f != nullptr) {
					for (auto i : f->explode() ){
						_particles.push_back(i);
					}
				}
				_forceReg.deleteParticleregistry(*it);
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
		_forceReg.addRegistry(gen, part);
		_particles.push_back(part);
		_forceGens.push_back(gen);
	}
	void generateWindDemo() {
		Particle* part = new Particle({ 10,10,10 }, { 0,0,0 }, { 0,0,0 },
			0.99, 10, 100000, { 1,0,0,1 }, CreateShape(physx::PxSphereGeometry(10)), true);
		WindForceGenerator* gen = new WindForceGenerator(Vector3(5, 0, 0), 1,0.1);
		_forceReg.addRegistry(gen, part);
		_particles.push_back(part);
		_forceGens.push_back(gen);
	}
	void generateTwisterDemo() {
		Particle* part = new Particle({ 0,0,0 }, { 0,0,0 }, { 0,0,0 },
			0.99, 10, 100000, { 1,0,0,1 }, CreateShape(physx::PxSphereGeometry(1)), false);
		GaussianParticleGenerator* g = new GaussianParticleGenerator("nombre", part, 100, { 20,20,20 }, { 0.00001,.000001,.00001 }, 100);
		
		Particle* part2 = new Particle({ 0,0,0 }, { 0,0,0 }, { 0,0,0 },
			0.99, 10, 100000, { 1,0,0,1 }, CreateShape(physx::PxBoxGeometry(1,1,1)), true);
		TwisterForceGenerator* gen = new TwisterForceGenerator(Vector3(0, 0, 0), 1, 0.1, 0.75);
		for (auto p : g->generateParticles()) {
			_particles.push_back(p);
			_forceReg.addRegistry( gen,p );
		}
		_forceReg.addRegistry(gen, part);
		_forceGens.push_back(gen);
		//_particles.push_back(part);
	}
	void generateExpDemo() {
		Particle* part = new Particle({ 0,0,0 }, { 0,0,0 }, { 0,0,0 }, 0.99, 1, 100000, { 0,0,1,1 });
		Particle* model = new Particle({ 0,0,0 }, { 0,0,0 }, { 0,0,0 }, 0.99, 10, 25, { 1,1,1,1 }, CreateShape(physx::PxSphereGeometry(1)), false);
		ParticleGenerator* gen = new GaussianParticleGenerator("JAJA", model, 100, {2,2,2},{0.00001,0.00001,0.000001},10);
		ExplosionForceGenerator* exp = new ExplosionForceGenerator({ 0,0,0 }, 10, 100, .25);
		for (auto p : gen->generateParticles()) {
			_forceReg.addRegistry(exp, p);
			_particles.push_back(p);
		}
		delete gen;
		_forceGens.push_back(exp);
	}

	void generateAnchoredSpringDemo() {
		Particle* p = new Particle({ 0,0,0 }, { 0,0,0 }, { 0,0,0 }, 0.99, 5, 1e10);
		AnchoredSpringFG* spring = new AnchoredSpringFG(Vector3(-10,0,0), 2, 5);
		_forceReg.addRegistry(spring, p);
		DragForceGenerator* drag = new DragForceGenerator(.5);
		_forceReg.addRegistry(drag, p);
		_particles.push_back(p);
		_forceGens.push_back(drag);
		_forceGens.push_back(spring);
	}

	void generateSpringDemo() {
		Particle* p1 = new Particle({ 10,0,0 }, { 0,0,0 }, { 0,0,0 }, 0.99, 5, 1e10);
		Particle* p2 = new Particle({ -10,0,0 }, { 0,0,0 }, { 0,0,0 }, 0.99, 5, 1e10, {1,0,1,1});
		SpringForceGenerator* spring1 = new SpringForceGenerator(p1, 2, 5);
		SpringForceGenerator* spring2 = new SpringForceGenerator(p2, 2, 5);
		_forceReg.addRegistry(spring1, p2);
		_forceReg.addRegistry(spring2, p1);
		DragForceGenerator* drag = new DragForceGenerator(.5);
		_forceReg.addRegistry(drag, p1);
		_forceReg.addRegistry(drag, p2);
		_particles.push_back(p1);
		_particles.push_back(p2);
		_forceGens.push_back(drag);
		_forceGens.push_back(spring1);
		_forceGens.push_back(spring2);
	}

	void generateBungeeDemo() {
		Particle* p1 = new Particle({ 10,0,0 }, { 0,0,0 }, { 0,0,0 }, 0.99, 5, 1e10);
		Particle* p2 = new Particle({ -10,0,0 }, { 0,0,0 }, { 0,0,0 }, 0.99, 5, 1e10, { 1,0,1,1 });
		BungeeForceGenerator* spring1 = new BungeeForceGenerator(p1, 2, 10);
		BungeeForceGenerator* spring2 = new BungeeForceGenerator(p2, 2, 10);
		_forceReg.addRegistry(spring1, p2);
		_forceReg.addRegistry(spring2, p1);
		DragForceGenerator* drag = new DragForceGenerator(.05);
		_forceReg.addRegistry(drag, p1);
		_forceReg.addRegistry(drag, p2);
		_particles.push_back(p1);
		_particles.push_back(p2);
		_forceGens.push_back(drag);
		_forceGens.push_back(spring1);
		_forceGens.push_back(spring2);
	}

	void generateBuoyancyDemo() {
		Particle* liquid = new Particle({ 0,0,0 }, { 0,0,0 }, { 0,0,0 }, 0.99, 0, 1e10, { 0,0,1,1 }, CreateShape(physx::PxBoxGeometry(20, 2, 20)));
		Particle* p = new Particle({ 0,-5, 0 }, { 0,0,0 }, { 0,0,0 }, 0.90, 20000, 1e10, { 1,0,0,1 }, CreateShape(physx::PxBoxGeometry(3, 3, 3)));

		BuoyancyForceGenerator* bg = new BuoyancyForceGenerator(5, 27, 1000, liquid);
		GravityForceGenerator* gg = new GravityForceGenerator(Vector3(0,-9.8,0), 0);
		_forceReg.addRegistry(bg, p);
		_forceReg.addRegistry(gg, p);
		_forceGens.push_back(gg);
		_forceGens.push_back(bg);
		_particles.push_back(p);
	}
	virtual ~ParticleSystem() {
		for (auto it = _particles.begin(); it != _particles.end(); it = _particles.erase(it)) {
			delete (*it);
		};
		for (auto gen = _generators.begin(); gen != _generators.end(); gen = _generators.erase(gen) ) {
			delete (*gen);
		}
		for (auto gen = _forceGens.begin(); gen != _forceGens.end(); gen = _forceGens.erase(gen)) {
			delete (*gen);
		}
	};
};