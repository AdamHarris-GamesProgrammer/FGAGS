#pragma once

#include "Particle.h"
#include "ParticleForceGenerator.h"

class ParticleGravity : public ParticleForceGenerator {
	Vector3 _gravity;

public:
	ParticleGravity(const Vector3& gravity) {
		_gravity = gravity;
	}

	virtual void Update(Particle* particle, float duration) {
		if (!particle->HasFiniteMass()) return;

		particle->AddForce(_gravity * particle->GetMass());
	}
};
