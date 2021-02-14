#include "Particle.h"

#include <string>
#include <Windows.h>
#include "Debug.h"

void Particle::Update(float dt)
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

	//Update velocity with acceleration
	_velocity += resultingAcceleration * dt;

	//Apply damping force, basic version of drag
	_velocity *= powf(_linearDamping, dt);

	//Clears accumulated forces at the end of each cycle in case a force is no longer being applied from a generator
	ClearAccumulator();
}

