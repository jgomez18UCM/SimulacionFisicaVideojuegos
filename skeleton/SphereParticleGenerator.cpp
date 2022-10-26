#include "SphereParticleGenerator.h"

SphereParticleGenerator::SphereParticleGenerator(int numParticles, Vector3 pos, Particle* model, float radius)
{
	_model = model;
	_mean_vel = model->getVel();
	_mean_pos = pos;
	_mean_acc = model->getAcc();
	_num_particles = numParticles;
	_radius = radius;
	
	random = std::uniform_real_distribution<float>(0, 1);
}

std::list<Particle*> SphereParticleGenerator::generateParticles()
{
	std::list<Particle*> l;
	std::random_device r{};
	std::mt19937 gen{r()};
	float theta = 0, phi = 0;
	for (int i = 0; i < _num_particles; i++)
	{
		phi = 2 * 3.14 * random(gen);
		theta = 2*3.14*random(gen);

		float x = cos(theta) * sin(phi);
		float y = sin(theta) * sin(phi);
		float z = cos(phi);

		Vector3 vel(x, y, z);
		Particle* p = _model->clone();
		p->setPos(_mean_pos);
		p->setVel(vel.getNormalized() * 20);
		l.push_back(p);
		
	}
	return l;
}