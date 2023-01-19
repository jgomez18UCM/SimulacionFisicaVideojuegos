#include "Firework.h"

Firework::Firework(Vector3 Pos, Vector3 Vel, Vector3 Acc, float damp, float mass, float time, Vector4 color, physx::PxShape* sh, bool render,  std::list<std::shared_ptr<ParticleGenerator>> gens) :
		Particle(Pos,Vel,Acc,damp,mass,time,color,sh,render), _gens(gens)
{
}

Firework::~Firework()
{}

std::list<Particle*> Firework::explode()
{
	std::list<Particle*> l;
	for (auto g : _gens) {
		g->setMeanPos(pose.p);
		for (auto p : g->generateParticles()) {
			l.push_back(p);
		}
	}
	return l;
}

Firework* Firework::cloneFirework()
{
	
	return new Firework(pose.p, vel, acc, damping, inverse_mass > 0 ? 1.0f / inverse_mass : 0.0f, timeLimit, color, shape, true, _gens);
}
