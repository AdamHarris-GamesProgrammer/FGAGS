#pragma once
#include "Precision.h"
#include "Particle.h"
#include "ParticleForceGenerator.h"

class ParticleBungee : public ParticleForceGenerator {
public:
	ParticleBungee(Particle* other, real springConstant, real restLength) {
		_other = other;
		_springConstant = springConstant;
		_restLength = restLength;
	}

	virtual void Update(Particle* particle, real duration) {
		//Calculate the vector of the spring
		Vector3 force;
		force = particle->GetPosition();
		force -= _other->GetPosition();

		//Calculate the magnitude of the force vector
		real magnitude = force.Magnitude();

		//if the magnitude is less than the rest length then dont add any force
		if (magnitude <= _restLength) return;


		magnitude = _springConstant * (_restLength - magnitude);

		//Calculate the final force and apply it
		force.Normalize();
		force *= -magnitude;
		particle->AddForce(force);

	}

private:
	Particle* _other;

	real _springConstant;

	real _restLength;
};
