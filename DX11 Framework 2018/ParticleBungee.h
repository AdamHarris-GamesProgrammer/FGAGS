#pragma once
#include "Particle.h"
#include "ParticleForceGenerator.h"

class ParticleBungee : public ParticleForceGenerator {
public:
	ParticleBungee(ParticleComponent* other, float springConstant, float restLength) {
		_other = other;
		_springConstant = springConstant;
		_restLength = restLength;
	}

	virtual void Update(ParticleComponent* particle, float duration) {
		//Calculate the vector of the spring
		Vector3 force;
		force = particle->GetPosition();
		force -= _other->GetPosition();

		//Calculate the magnitude of the force vector
		float magnitude = force.Magnitude();

		//if the magnitude is less than the rest length then dont add any force
		if (magnitude <= _restLength) return;
		

		magnitude = _springConstant * (_restLength - magnitude);

		//Calculate the final force and apply it
		force.Normalize();
		force *= -magnitude;
		particle->AddForce(force);

	}

	float GetSpringConstant() const {
		return _springConstant;
	}

	float GetRestLength() const {
		return _restLength;
	}

	void SetSpringConstant(float val) {
		_springConstant = val;
	}

	void SetRestLength(float val) {
		_restLength = val;
	}

private:
	ParticleComponent* _other;

	float _springConstant;

	float _restLength;
};
