#include "Particle.h"

#include <string>
#include <Windows.h>
#include "Debug.h"

void Particle::Update(real dt)
{
	//Immovable object check
	if (GetInverseMass() <= 0.0) return;

	assert(dt > 0.0);

	//Stops moving at a certain point to reduce calculations required
	//if (_velocity < 0.05) return;

	//_velocity.Print("Velocity ");

	//Update position

	if (_velocity > 0.05) {
		_position += _velocity * dt;
	}

	Vector3 resultingAcceleration = _acceleration;

	//Adds accumulated force from the De Alembert principle
	resultingAcceleration += _forceAccumulator * _inverseMass;

	_forceAccumulator.Print("Spring Force Test: ");

	//Update velocity with acceleration
	_velocity += resultingAcceleration * dt;

	//Apply damping force, basic version of drag
	_velocity *= real_pow(_damping, dt);

	//Clears accumulated forces at the end of each cycle in case a force is no longer being applied from a generator
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