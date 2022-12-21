#pragma once
#include <map>
#include "RigidBodyFG.h"
#include "RigidBody.h"
#include "core.hpp"

class RBForceRegistry : std::multimap<RigidBodyFG*, RigidBody*>
{
public:
	void updateForces(float duration) {
		for (auto it = begin(); it != end(); ++it) {
			it->first->updateForce(it->second, duration);
		}
	}

	void addRegistry(RigidBodyFG* fg, RigidBody* rg) {
		insert({ fg, rg });
	}

	void deleteRigidRegistry(RigidBody* rb) {
		for (auto it = begin(); it != end();) {
			if (it->second == rb) {
				it = erase(it);
			}
			else {
				++it;
			}
		}
	}
};

