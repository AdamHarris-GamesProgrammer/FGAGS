#pragma once
#include "Precision.h"
#include "Particle.h"
#include "ParticleForceGenerator.h"

class ParticleAnchoredSpring : public ParticleForceGenerator {
public:
	ParticleAnchoredSpring(Vector3* anchor, real springConstant, real restLength) {
		_anchorLocation = new Vector3(*anchor);
		
		_springConstant = springConstant;
		_restLength = restLength;
	}

	virtual void Update(Particle* particle, real duration) {
		//Calculate the direction between the spring and the anchor
		Vector3 force;
		force = particle->GetPosition();
		force -= *_anchorLocation;

		//Calculate Magnitude of the force vector
		real magnitude = force.Magnitude();
		magnitude = (_restLength - magnitude) * _springConstant;

		//Calculate the final force applied
		force.Normalize();
		force *= magnitude;
		particle->AddForce(force);
	}

	void SetAnchorLocation(Vector3* val) { _anchorLocation = val; }

protected:
	Vector3* _anchorLocation;

	real _springConstant;

	real _restLength;
};