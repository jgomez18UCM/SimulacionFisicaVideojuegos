#include "Player.h"
#include <iostream>
Player::Player(RigidBody* RB, Camera* cam)
{
	_rb = RB;
	_rb->unrender();
	
	_cam = cam;
	direction = _cam->getDir();
}

void Player::update()
{
	/*auto veloc = _rb->getLinearVelocity();
	auto velY = veloc.y;
	
	veloc = { vel.x,velY,vel.z };
	_rb->setLinearVelocity(veloc);*/
	//std::cout << _rb->getLinearVelocity().x << " " << _rb->getLinearVelocity().y << " "<<_rb->getLinearVelocity().z<<"\n";
	_cam->setPosition(_rb->getPosition());
}

bool Player::handleKey(unsigned char key)
{
	direction = _cam->getDir();
	direction.y = 0;
	std::cout << direction.x << " " << direction.y << " " << direction.z << "\n";
	Vector3 viewY = direction.cross(Vector3(0, 1, 0)).getNormalized();
	auto mass = _rb->getMass();
	auto vel = _rb->getLinearVelocity();
	auto velY = vel.y;
	switch (toupper(key))
	{
	case 'W':	vel = direction * maxVel;		break;
	case 'S':	vel = -direction * maxVel;		break;
	case 'A':	vel = -viewY * maxVel;		break;
	case 'D':	vel = viewY * maxVel;		break;
	default:							return false;
	}
	std::cout << vel.x << " " << vel.y << " " << vel.z << "\n";
	vel.y = 0;
	_rb->setLinearVelocity(vel);
	std::cout << "RBVEL:" << _rb->getLinearVelocity().x << " " << _rb->getLinearVelocity().y << " " << _rb->getLinearVelocity().z << "\n";
	return true;
}
