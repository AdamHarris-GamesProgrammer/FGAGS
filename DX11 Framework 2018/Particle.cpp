#include "Particle.h"

#include <string>
#include <Windows.h>

void Particle::Update(real dt)
{
	//Immovable object check
	if (GetInverseMass() <= 0.0) return;

	assert(dt > 0.0);

	//Gets the position
	_position = _transform->GetPosition();

	//Update position
	_position += _velocity * dt;

	Vector3 resultingAcceleration = _acceleration;

	//Adds accumulated force from De Alemberts principle
	resultingAcceleration += _forceAccumulator * _inverseMass;

	//char sz[1024] = { 0 };
	//sprintf_s(sz, "Velocity: %f\n", _velocity.y);
	//OutputDebugStringA(sz);

	//Update velocity with acceleration
	_velocity += resultingAcceleration * dt;



	//Apply damping force, basic version of drag
	_velocity *= real_pow(_damping, dt);

	//Sets position
	_transform->SetPosition(_position);

	ClearAccumulator();
}

void Particle::ClearAccumulator()
{
	_forceAccumulator.Zero();
}

void Particle::AddForce(const Vector3& force)
{
	_forceAccumulator += force;
}

real Particle::GetMass() const
{
	if (_inverseMass == 0) {
		return REAL_MAX;
	}
	else {
		return ((real)1.0) / _inverseMass;
	}
}

void Particle::SetMass(const real mass)
{
	assert(mass != 0);
	_inverseMass = ((real)1.0) / mass;
}
