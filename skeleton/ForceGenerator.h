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

class ExplosionForceGenerator : public ForceGenerator {
public:
	ExplosionForceGenerator(Vector3 center, float radius, float k, double kt) : _center(center), _radius(radius), _k(k), _kt(kt), _timePassed(0.0) {};
	~ExplosionForceGenerator() {};
	void updateForce(Particle* p, double duration) override;
protected:
	Vector3 _center;
	float _radius;
	float _k;
	double _kt;
	double _timePassed;
};

class SpringForceGenerator : public ForceGenerator {
public:
	SpringForceGenerator(Particle* other, float k, float resting_length);
	virtual ~SpringForceGenerator() {};
	void updateForce(Particle* p, double duration) override;

protected:
	float _k;
	float _resting_length;
	Particle* _other;
};

class AnchoredSpringFG : public SpringForceGenerator {
public:
	AnchoredSpringFG(Vector3& position, float k, float resting_length);
	~AnchoredSpringFG();
};

