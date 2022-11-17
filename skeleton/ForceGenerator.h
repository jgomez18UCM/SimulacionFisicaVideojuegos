#pragma once
#include "Particle.h"
#include <list>
#include <string>

class ForceGenerator
{
public:
	virtual void updateForce(Particle* particle, double duration) = 0;
	std::string name;
	double t = -1;
};

class GravityForceGenerator : public ForceGenerator {
public:
	GravityForceGenerator(Vector3 & G, double t);
	virtual ~GravityForceGenerator() {};
	void updateForce(Particle* particle, double duration) override;
protected:
	Vector3 g;
};

class DragForceGenerator : public ForceGenerator {
public:
	DragForceGenerator(float k) : k(k){};
	virtual ~DragForceGenerator() {};
	void updateForce(Particle* particle, double duration) override;
protected:
	float k;
};

class WindForceGenerator : public ForceGenerator {
public:
	WindForceGenerator(Vector3& v, float k1, float k2);
	virtual ~WindForceGenerator() {};
	void updateForce(Particle* particle, double duration) override;
protected:
	Vector3 _windVel;
	float _k1;
	float _k2;
};

class TwisterForceGenerator : public WindForceGenerator {
public:
	TwisterForceGenerator(Vector3 center, float k1, float k2, float kt) : WindForceGenerator(Vector3(0), k1, k2), _center(center), _Kt(kt) {};
	~TwisterForceGenerator() {};
	void updateForce(Particle* particle, double duration) override;
protected:
	Vector3 _center;
	float _Kt;
};

