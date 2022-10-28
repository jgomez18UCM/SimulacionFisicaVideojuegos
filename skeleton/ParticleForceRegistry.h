#pragma once
#include "ForceGenerator.h"
#include "Particle.h"
#include <map>
typedef std::pair<ForceGenerator*, Particle*> FRpair;

class ParticleForceRegistry : public std::multimap<ForceGenerator*, Particle*>
{
public:
	void updateForces(float duration) {
		for (auto it = begin(); it != end(); ++it) {
			it->first->updateForce(it->second, duration);
		}
	}

	void addRegistry(ForceGenerator* fg, Particle* p) {
		insert({ fg, p });
	}

	void deleteParticleregistry(Particle* p) {
		for (auto it = begin(); it != end();) {
			if (it->second == p) {
				it = erase(it);
			}
			else {
				++it;
			}
		}
	}
};

