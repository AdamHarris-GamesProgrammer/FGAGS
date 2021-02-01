#include "Particle.h"

void Particle::Integrate(real dt)
{
	_position = _transform->GetPosition();

	if (InverseMass() <= 0.0) return;

	assert(dt > 0.0);

	_position.AddScaledVector(_velocity, dt);

	Vector3 resultingAcceleration = _acceleration;

	Velocity().AddScaledVector(resultingAcceleration, dt);

	Velocity(_velocity * real_pow(_damping, dt));

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

real Particle::Mass() const
{
	if (_inverseMass == 0) {
		return REAL_MAX;
	}
	else {
		return ((real)1.0) / _inverseMass;
	}
}

void Particle::Mass(const real mass)
{
	assert(mass != 0);
	_inverseMass = ((real)1.0) / mass;
}
