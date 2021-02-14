#pragma once
#include "Particle.h"
#include "ParticleForceGenerator.h"

class ParticleSpring : public ParticleForceGenerator {
public:
	ParticleSpring(Particle* other, float springConstant, float restLength) {
		_other = other;
		_springConstant = springConstant;
		_restLength = restLength;
	}

	virtual void Update(Particle* particle, float duration) {
		//Calculate the spring vector
		Vector3 force;
		force = particle->GetPosition();
		force -= _other->GetPosition();

		//Calculate the magnitude of the force
		float magnitude = force.Magnitude();
		magnitude = fabsf(magnitude - _restLength);
		magnitude *= _springConstant;

		//Calculates the final force and applies it
		force.Normalize();
		force *= -magnitude;
		particle->AddForce(force);
	}


private:
	Particle* _other;

	float _springConstant;
	float _restLength;
};