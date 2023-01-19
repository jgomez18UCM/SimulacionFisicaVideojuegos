#pragma once
#include "./Render/Camera.h"
#include "core.hpp"
#include "RigidBody.h"
class Player
{
public:
	Player(RigidBody* RB, Camera* cam);
	virtual ~Player() {};
	void update();
	bool handleKey(unsigned char key);
private:
	Camera* _cam;
	RigidBody* _rb;
	float acc = 1;
	float maxVel = 10;
	Vector3 vel = { 0,0,0 };
	Vector3 direction;
};

