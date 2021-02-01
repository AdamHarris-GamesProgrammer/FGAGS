#include "Particle.h"

void Particle::Integrate(real dt)
{
	_position = _transform->GetPosition();

	if (GetInverseMass() <= 0.0) return;

	assert(dt > 0.0);

	_position.AddScaledVector(_velocity, dt);

	Vector3 resultingAcceleration = _acceleration;

	_velocity.AddScaledVector(resultingAcceleration, dt);

	_velocity *= real_pow(_damping, dt);

	_transform->SetPosition(_position);

	ClearAccumulator();
}

void Particle::ClearAccumulator()
{
	_forceAccumulator = Vector3();
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
