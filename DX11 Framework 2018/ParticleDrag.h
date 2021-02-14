#pragma once
#include "Particle.h"
#include "ParticleForceGenerator.h"

class ParticleDrag : public ParticleForceGenerator {
	//Velocity drag coefficient
	float _dragCoefficient;

	//Velocity squared drag coefficient
	float _squaredDragCoefficient;

public:
	ParticleDrag(float coefficient, float squaredCoefficient) {
		_dragCoefficient = coefficient;
		_squaredDragCoefficient = squaredCoefficient;
	}

	virtual void Update(Particle* particle, float duration) {
		Vector3 totalDrag;
		totalDrag = particle->GetVelocity();

		//Calculate drag factor
		float dragFactor = totalDrag.Magnitude();
		dragFactor = _dragCoefficient * dragFactor + _squaredDragCoefficient * dragFactor * dragFactor;

		//Calculate total drag
		totalDrag.Normalize();
		totalDrag *= -dragFactor;

		//Add the drag to the particle
		particle->AddForce(totalDrag);
	}

	void SetDrags(float drag, float squaredDrag) {
		_dragCoefficient = drag;
		_squaredDragCoefficient = squaredDrag;
	}

	void SetDragCoefficient(float drag) {
		_dragCoefficient = drag;
	}

	void SetSquaredDragCoefficient(float drag) {
		_squaredDragCoefficient = drag;
	}
};
