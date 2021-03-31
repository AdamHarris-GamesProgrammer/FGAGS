#pragma once
#include "Particle.h"
#include "ParticleForceGenerator.h"

class ParticleAnchoredSpring : public ParticleForceGenerator {
public:
	ParticleAnchoredSpring(Vector3* anchor, float springConstant, float restLength) {
		_anchorLocation = new Vector3(*anchor);
		
		_springConstant = springConstant;
		_restLength = restLength;
	}

	virtual void Update(ParticleComponent* particle, float duration) {
		//Calculate the direction between the spring and the anchor
		Vector3 force;
		force = particle->GetPosition();
		force -= *_anchorLocation;

		//Calculate Magnitude of the force vector
		float magnitude = force.Magnitude();
		magnitude = (_restLength - magnitude) * _springConstant;

		//Calculate the final force applied
		force.Normalize();
		force *= magnitude;
		particle->AddForce(force);
	}

	void SetAnchorLocation(Vector3* val) { _anchorLocation = val; }

protected:
	Vector3* _anchorLocation;

	float _springConstant;

	float _restLength;
};