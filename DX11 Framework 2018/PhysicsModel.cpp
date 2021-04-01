#include "PhysicsModel.h"

void PhysicsModelComponent::AddForce(const Vector3& force)
{
	_forceAccumulator += force;
}

void PhysicsModelComponent::ClearAccumulator()
{
	_forceAccumulator.Zero();
}

float PhysicsModelComponent::GetMass() const
{
	if (_inverseMass == 0) {
		return FLT_MAX;
	}
	else {
		return ((float)1.0) / _inverseMass;
	}
}

void PhysicsModelComponent::SetMass(const float mass)
{
	assert(mass != 0);
	_inverseMass = ((float)1.0) / mass;
}

float PhysicsModelComponent::GetInverseMass() const
{
	return _inverseMass;
}

void PhysicsModelComponent::SetInverseMass(float val)
{
	_inverseMass = val;
}

float PhysicsModelComponent::GetDamping() const
{
	return _linearDamping;
}

void PhysicsModelComponent::SetDamping(float val)
{
	_linearDamping = val;
}

Vector3 PhysicsModelComponent::GetAcceleration() const
{
	return _acceleration;
}

void PhysicsModelComponent::SetAcceleration(Vector3 val /*= Vector3()*/)
{
	_acceleration = val;
}

void PhysicsModelComponent::SetAcceleration(float x, float y, float z)
{
	_acceleration.x = x; _acceleration.y = y; _acceleration.z = z;
}

Vector3 PhysicsModelComponent::GetVelocity() const
{
	return _velocity;
}

void PhysicsModelComponent::SetVelocity(Vector3 val /*= Vector3()*/)
{
	_velocity = val;
}

void PhysicsModelComponent::SetVelocity(float x, float y, float z)
{
	_velocity.x = x; _velocity.y = y; _velocity.z = z;
}

Vector3 PhysicsModelComponent::GetPosition() const
{
	return _position;
}

void PhysicsModelComponent::SetPosition(Vector3 val /*= Vector3()*/)
{
	_position = val;
}

void PhysicsModelComponent::SetPosition(float x, float y, float z)
{
	_position = Vector3(x, y, z);
}

void PhysicsModelComponent::Initialize()
{
	SetMass(5.0);
	SetVelocity(0, 0, 0);
}
