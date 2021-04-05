#pragma once

#include "Particle.h"

class ParticleGravity {
	Vector3 _gravity;

public:
	ParticleGravity(const Vector3& gravity) {
		_gravity = gravity;
	}

	void Update(ParticleComponent* particle, float duration) {
		if (!particle->HasFiniteMass()) return;

		particle->AddForce(_gravity * particle->GetMass());
	}
};
