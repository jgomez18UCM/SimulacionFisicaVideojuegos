#pragma once
#include "ParticleGenerator.h"
#include <random>
class GaussianParticleGenerator : public ParticleGenerator {
public:
	GaussianParticleGenerator(std::string name, physx::PxShape* model, Vector4 color, Vector3 meanPos, Vector3 meanVel,
		Vector3 meanAcc, double genProb, Vector3 posDeviation, Vector3 velDeviation, int numPart) {
		setName(name);
		_shape = model;
		_modelColor = color;
		_mean_pos = meanPos;
		_mean_vel = meanVel;
		_mean_acc = meanAcc;
		_generation_probability = genProb;
		_std_dev_vel = velDeviation;
		_std_dev_pos = posDeviation;
		_num_particles = numPart;
		std::random_device r;
		random_generator = std::mt19937(r());
	}

	std::list<Particle*> generateParticles() override {
		std::list<Particle*> l;
		auto px = std::normal_distribution<float>(_mean_pos.x, _std_dev_pos.z);
		auto py = std::normal_distribution<float>(_mean_pos.y, _std_dev_pos.z);
		auto pz = std::normal_distribution<float>(_mean_pos.z, _std_dev_pos.z);
		auto gen = std::uniform_int_distribution<int>(0, 100);
		auto vx = std::normal_distribution<float>(_mean_vel.x, _std_dev_vel.x);
		auto vy = std::normal_distribution<float>(_mean_vel.y, _std_dev_vel.y);
		auto vz = std::normal_distribution<float>(_mean_vel.z, _std_dev_vel.z);
		
		
		for (int i = 0; i < _num_particles; i++) {
			int cr = gen(random_generator);
			if (cr <= _generation_probability)
			{
				Vector3 pos = { px(random_generator), py(random_generator), pz(random_generator) };
				Vector3 vel = { vx(random_generator), vy(random_generator), vz(random_generator) };

				Particle* p = new Particle(pos, vel, _mean_acc, 0.99, _modelColor,  _shape);
				l.push_back(p);
			}
		}
		return l;
	};
protected:
	std::mt19937 random_generator;
	Vector3 _std_dev_vel, _std_dev_pos;
};