#include "Particle.h"

#include <string>
#include <Windows.h>
#include "Debug.h"
#include "Object.h"

void ParticleComponent::Update(float dt)
{
	TransformComponent* pTransformComponent = dynamic_cast<TransformComponent*>(_pOwner->GetComponent(Transform));
	_position = pTransformComponent->GetPosition();

	//Immovable object check
	if (GetInverseMass() <= 0.0) return;

	assert(dt > 0.0);

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

	_position.AddScaledVector(_velocity, dt);

	//Clears accumulated forces at the end of each cycle in case a force is no longer being applied from a generator
	ClearAccumulator();

	pTransformComponent->SetPosition(_position);

	CheckSleep(_velocity.ScalarProduct(_velocity), dt);
}

