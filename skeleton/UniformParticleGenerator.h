#pragma once
#include "ParticleGenerator.h"
#include <random>

class UniformParticleGenerator : public ParticleGenerator {

public:
	UniformParticleGenerator(std::string name, Particle* model, Vector3 meanPos, double genProb, Vector3 posWidth, Vector3 velWidth, int numPart) {
		setName(name);
		setParticle(model);
		/*_shape = model;
		_modelColor = modelColor;*/
		_mean_pos = _model->getPos();
		_mean_vel = _model->getVel();
		//_mean_acc = meanAcc;
		_generation_probability = genProb;
		_vel_width = velWidth;
		_pos_width = posWidth;
		_num_particles = numPart;
		std::random_device r;
		random_generator = std::mt19937(r());
	}
	std::list<Particle*> generateParticles() override{
		std::list<Particle*> l;
		auto px = std::uniform_real_distribution<float>(_mean_pos.x - _pos_width.x / 2, _mean_pos.x + _pos_width.x / 2);
		auto py = std::uniform_real_distribution<float>(_mean_pos.y - _pos_width.y / 2, _mean_pos.y + _pos_width.y / 2);
		auto pz = std::uniform_real_distribution<float>(_mean_pos.z - _pos_width.z / 2, _mean_pos.z + _pos_width.z / 2);
		auto gen = std::uniform_int_distribution<int>(0, 100);
		auto vx = std::uniform_real_distribution<float>(_mean_vel.x - _vel_width.x / 2, _mean_vel.x + _vel_width.x / 2);
		auto vy = std::uniform_real_distribution<float>(_mean_vel.y - _vel_width.y / 2, _mean_vel.y + _vel_width.y / 2);
		auto vz = std::uniform_real_distribution<float>(_mean_vel.z - _vel_width.z / 2, _mean_vel.z + _vel_width.z / 2);
		for (int i = 0; i < _num_particles; i++) {		
			int cr = gen(random_generator);
			if(cr <= _generation_probability)
			{
				Vector3 pos = { px(random_generator), py(random_generator), pz(random_generator) };
				Vector3 vel = { vx(random_generator), vy(random_generator), vz(random_generator) };

				Particle* p = _model->clone();
				p->setVel(vel);
				p->setPos(pos);
				l.push_back(p);
			}
		}
		return l;
	};
	virtual ~UniformParticleGenerator() {};
private:
	Vector3 _vel_width, _pos_width;
	std::mt19937 random_generator;
	
};