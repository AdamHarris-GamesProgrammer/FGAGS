#include "PhysicsModel.h"

void PhysicsModel::AddForce(const Vector3& force)
{
	_forceAccumulator += force;
}

void PhysicsModel::ClearAccumulator()
{
	_forceAccumulator.Zero();
}

real PhysicsModel::GetMass() const
{
	if (_inverseMass == 0) {
		return REAL_MAX;
	}
	else {
		return ((real)1.0) / _inverseMass;
	}
}

void PhysicsModel::SetMass(const real mass)
{
	assert(mass != 0);
	_inverseMass = ((real)1.0) / mass;
}

real PhysicsModel::GetInverseMass() const
{
	return _inverseMass;
}

void PhysicsModel::SetInverseMass(real val)
{
	_inverseMass = val;
}

real PhysicsModel::GetDamping() const
{
	return _linearDamping;
}

void PhysicsModel::SetDamping(real val)
{
	_linearDamping = val;
}

Vector3 PhysicsModel::GetAcceleration() const
{
	return _acceleration;
}

void PhysicsModel::SetAcceleration(Vector3 val /*= Vector3()*/)
{
	_acceleration = val;
}

void PhysicsModel::SetAcceleration(float x, float y, float z)
{
	_acceleration.x = x; _acceleration.y = y; _acceleration.z = z;
}

Vector3 PhysicsModel::GetVelocity() const
{
	return _velocity;
}

void PhysicsModel::SetVelocity(Vector3 val /*= Vector3()*/)
{
	_velocity = val;
}

void PhysicsModel::SetVelocity(float x, float y, float z)
{
	_velocity.x = x; _velocity.y = y; _velocity.z = z;
}

Vector3 PhysicsModel::GetPosition() const
{
	return _position;
}

void PhysicsModel::SetPosition(Vector3 val /*= Vector3()*/)
{
	_position = val;
}

void PhysicsModel::SetPosition(float x, float y, float z)
{
	_position = Vector3(x, y, z);
}
