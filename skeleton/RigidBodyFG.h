#pragma once
#include <string>
#include "core.hpp"
#include "RigidBody.h"

struct Restraint {
	float maxX, minX;
	float maxY, minY;
	float maxZ, minZ;
	bool defecto = true;
};

class RigidBodyFG
{
protected:
	Restraint _res = {};
public:
	virtual void updateForce(RigidBody* rb, double t) = 0;
	virtual void setRestraint(Restraint& res) { _res = res; };
	virtual bool checkRes(Vector3& pos);
	std::string name;
	double t = -1;
	
};

class RigidBodyWindFG : public RigidBodyFG {
public:
	RigidBodyWindFG(Vector3 windVel, float k1, float k2);
	void updateForce(RigidBody* rb, double t) override;
protected:
	float _k1, _k2;
	Vector3 _windVel;
};

class RBTwisterFG : public RigidBodyWindFG {
public:
	RBTwisterFG(Vector3 center, float k1, float k2, float kt) : RigidBodyWindFG(Vector3(0), k1, k2), _center(center), _Kt(kt) {};
	~RBTwisterFG() {};
	void updateForce(RigidBody* rb, double duration) override;
protected:
	Vector3 _center;
	float _Kt;
};