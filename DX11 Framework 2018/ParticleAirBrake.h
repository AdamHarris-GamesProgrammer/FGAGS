#pragma once
#include "Particle.h"
#include "ParticleForceGenerator.h"
#include "Precision.h"

class ParticleAirBrake : public ParticleForceGenerator {
public:
	ParticleAirBrake(Vector3 force, bool activated = false) {
		isActivated = activated;
		_brakingForce = force;
	}

	virtual void Update(Particle* particle, real duration) {
		if (isActivated) {
			particle->AddForce(_brakingForce - particle->GetVelocity());
		}
	}

	bool IsActive() const { return isActivated; }
	void SetActive(bool active) { isActivated = active; }

private:
	bool isActivated;
	Vector3 _brakingForce;
};
