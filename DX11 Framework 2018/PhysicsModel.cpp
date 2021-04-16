#include "PhysicsModel.h"

void PhysicsModelComponent::AddForce(const Vector3& force)
{
	_forceAccumulator += force;
	_isAwake = true;
}

bool PhysicsModelComponent::BeginUpdate(float dt)
{
	_position = _pTransformComponent->GetPosition();

	//Immovable object check
	if (GetInverseMass() <= 0.0) return false;

	assert(dt > 0.0);

	if (!_isAwake) return false;

	return true;
}

void PhysicsModelComponent::EndUpdate(float currMot, float dt)
{
	//Clears accumulated forces at the end of each cycle in case a force is no longer being applied from a generator
	ClearAccumulator();
	CheckSleep(currMot, dt);

	//Sets the transforms position
	_pTransformComponent->SetPosition(_position);
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
		return 1.0f / _inverseMass;
	}
}

void PhysicsModelComponent::SetMass(const float mass)
{
	assert(mass != 0);
	_inverseMass = 1.0f / mass;
}

bool PhysicsModelComponent::HasFiniteMass() const
{
	return _inverseMass >= 0.0f;
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

bool PhysicsModelComponent::GetAwake() const
{
	return _isAwake;
}

float PhysicsModelComponent::GetSleepEpsilon()
{
	return _sleepEpsilon;
}

void PhysicsModelComponent::SetAwake(const bool awake /*= true*/)
{
	if (awake) {
		_isAwake = true;
		_motion = _sleepEpsilon * 2.0f;
	}
	else
	{
		_isAwake = false;
		_velocity.Zero();
	}
}

void PhysicsModelComponent::SetCanSleep(const bool canSleep)
{
	_canSleep = canSleep;

	//Makes the object wake up if it is asleep and we can no longer sleep
	if (!canSleep && !_isAwake) SetAwake();
}

void PhysicsModelComponent::Initialize()
{
	//gets the transform component
	_pTransformComponent = dynamic_cast<TransformComponent*>(_pOwner->GetComponent(ComponentID::Transform));

	SetCanSleep(true);
	SetAwake(true);
	SetMass(5.0);
	SetVelocity(0, 0, 0);
	SetDamping(0.9f);
}

void PhysicsModelComponent::CheckSleep(float currMot, float dt)
{
	//if we can sleep
	if (_canSleep) {
		//Get the current motion
		float currentMotion = currMot;
		float bias = powf(0.5, dt);

		_motion = bias * _motion + (1 - bias) * currentMotion;

		//object can sleep
		if (_motion < _sleepEpsilon) SetAwake(false);

		//object dosent need to sleep
		else if (_motion > 10 * _sleepEpsilon) _motion = 10 * _sleepEpsilon;
	}
}
