#pragma once
#include "Precision.h"
#include "Particle.h"
#include "ParticleForceGenerator.h"

class ParticleSpring : public ParticleForceGenerator {
public:
	ParticleSpring(Particle* other, real springConstant, real restLength) {
		_other = other;
		_springConstant = springConstant;
		_restLength = restLength;
	}

	virtual void Update(Particle* particle, real duration) {
		//Calculate the spring vector
		Vector3 force;
		force = particle->GetPosition();
		force -= _other->GetPosition();

		//Calculate the magnitude of the force
		real magnitude = force.Magnitude();
		magnitude = real_abs(magnitude - _restLength);
		magnitude *= _springConstant;

		//Calculates the final force and applies it
		force.Normalize();
		force *= -magnitude;
		particle->AddForce(force);
	}


private:
	Particle* _other;

	real _springConstant;
	real _restLength;
};