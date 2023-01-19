#include "RigidBody.h"
#include <iostream>

RigidBody::RigidBody(physx::PxRigidDynamic* rigid, double time, Vector4 color) {
	_rigid = rigid;
	_lifeTime = time;
	_life = 0;
	physx::PxShape* shape;
	_rigid->getShapes(&shape, 1);
	_render_item = new RenderItem(shape, _rigid, color);
	
}

RigidBody::RigidBody(physx::PxScene* scene, physx::PxPhysics* physx,  Vector3 pos, Vector3 vel, double mass, double time, Vector3 dims, Shape s, Vector4 color)
{
	_rigid = physx->createRigidDynamic(physx::PxTransform(pos));
	_rigid->setLinearVelocity(vel);
	_rigid->setMass(mass);
	_rigid->setAngularVelocity({ 0,0,0 });
	_lifeTime = time;
	_life = 0;
	
	physx::PxShape* shape;
	switch (s) {
		case Box:
			shape = CreateShape(physx::PxBoxGeometry(dims));
			break;
		case Sphere:
			shape = CreateShape(physx::PxSphereGeometry(dims.x));
			break;
		case Capsule:
			shape = CreateShape(physx::PxCapsuleGeometry(dims.x, dims.y));
			break;
		default:
			shape = CreateShape(physx::PxSphereGeometry(1));
			break;
	}

	_rigid->attachShape(*shape);
	physx::PxRigidBodyExt::updateMassAndInertia(*_rigid,1);
	scene->addActor(*_rigid);
	_render_item = new RenderItem(shape, _rigid, color);
}

RigidBody::~RigidBody() {
	_rigid->release();
	if(_render_item!=nullptr)DeregisterRenderItem(_render_item);
}

void RigidBody::update(double t) {
	if (_lifeTime > 0.0) _life += t;
}

bool RigidBody::isAlive() {
	if (_lifeTime < 0) return true;
	return _life < _lifeTime;
}

void RigidBody::addForce(Vector3 f) {
	_rigid->addForce(f);
}

void RigidBody::addTorque(Vector3 f) {
	_rigid->addTorque(f);
}

Vector3 RigidBody::getPosition() const
{
	return _rigid->getGlobalPose().p;
}

Vector3 RigidBody::getLinearVelocity() const
{
	return _rigid->getLinearVelocity();
}

float RigidBody::getMass() const
{
	return _rigid->getMass();
}

float RigidBody::getInvMass() const
{
	if (_rigid->getMass() <= 0) return -1;
	return _rigid->getInvMass();
}

void RigidBody::setLinearVelocity(Vector3& vel)
{
	_rigid->setLinearVelocity(vel);
}

void RigidBody::unrender()
{
	if (_render_item != nullptr) {
		DeregisterRenderItem(_render_item);
		_render_item = nullptr;
	}
}


