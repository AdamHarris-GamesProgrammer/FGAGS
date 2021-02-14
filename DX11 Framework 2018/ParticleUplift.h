#pragma once
#include "Particle.h"
#include "ParticleForceGenerator.h"

class ParticleUplift : public ParticleForceGenerator {
public:
	ParticleUplift(Vector3 origin, float radius, Vector3 uplift = Vector3(0.0,10.0,0.0)) {
		_origin = origin;
		_radius = radius;
		_upliftForce = uplift;
	}

	virtual void Update(Particle* particle, float duration) {
		float distance = particle->GetPosition().XZDistance(_origin);

		if (distance <= _radius) {
			particle->AddForce(_upliftForce);
		}
	}

private:
	Vector3 _upliftForce;
	Vector3 _origin;
	float _radius;
};
