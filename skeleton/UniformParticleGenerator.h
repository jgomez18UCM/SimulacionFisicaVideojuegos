#pragma once
#include "ParticleGenerator.h"
#include <random>

class UniformParticleGenerator : public ParticleGenerator {

public:
	UniformParticleGenerator(std::string name, Particle* model, Vector3 meanPos, Vector3 meanVel, double genProb,
							Vector3 velWidth, Vector3 posWidth, int numPart) {
		setName(name);
		setParticle(model);
		_mean_pos = meanPos;
		_mean_vel = meanVel;
		_generation_probability = genProb;
		_vel_width = velWidth;
		_pos_width = posWidth;
		_num_particles = numPart;
		std::random_device r;
		e3 = std::mt19937(r());
	}
	virtual std::list<Particle*> generateParticles() {
		std::list<Particle*> l;
		for (int i = 0; i < _num_particles; i++) {	
			std::default_random_engine generator(10);
			auto px = std::uniform_real_distribution<float>(_mean_pos.x - _pos_width.x / 2, _mean_pos.x + _pos_width.x / 2);
			auto py = std::uniform_real_distribution<float>(_mean_pos.y - _pos_width.y / 2, _mean_pos.y + _pos_width.y / 2);
			auto pz = std::uniform_real_distribution<float>(_mean_pos.z - _pos_width.z / 2, _mean_pos.z + _pos_width.z / 2);

			auto vx = std::uniform_real_distribution<float>(_mean_pos.x - _vel_width.x / 2, _mean_vel.x + _vel_width.x / 2);
			auto vy = std::uniform_real_distribution<float>(_mean_pos.y - _vel_width.y / 2, _mean_vel.y + _vel_width.y / 2);
			auto vz = std::uniform_real_distribution<float>(_mean_pos.z - _vel_width.z / 2, _mean_vel.z + _vel_width.z / 2);
			Vector3 pos = { px(e3), py(e3), pz(e3) };
			Vector3 vel = { vx(e3), vy(e3), vz(e3) };

			Particle* p = new Particle(pos, vel, { 0,0,0 }, 0.999);
			l.push_back(p);
		}
	};
	virtual ~UniformParticleGenerator() {};
private:
	Vector3 _vel_width, _pos_width;
	std::mt19937 e3;
};