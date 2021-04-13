#include "Particle.h"

#include <string>
#include <Windows.h>
#include "Debug.h"
#include "Object.h"

void ParticleComponent::Update(float dt)
{
	if (!BeginUpdate(dt)) return;

	Vector3 resultingAcceleration = _acceleration;

	//Adds accumulated force from the De Alembert principle
	resultingAcceleration += _forceAccumulator * _inverseMass;

	//Update velocity with acceleration
	_velocity += resultingAcceleration * dt;

	//Apply damping force, basic version of drag
	_velocity *= powf(_linearDamping, dt);

	//Calculates position
	_position += _velocity * dt;
	

	EndUpdate(_velocity.ScalarProduct(_velocity), dt);
}

