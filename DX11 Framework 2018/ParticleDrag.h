#pragma once
#include "Particle.h"
#include "ParticleForceGenerator.h"
#include "Precision.h"

class ParticleDrag : public ParticleForceGenerator {
	//Velocity drag coefficient
	real _dragCoefficient;

	//Velocity squared drag coefficient
	real _squaredDragCoefficient;

public:
	ParticleDrag(real coefficient, real squaredCoefficient) {
		_dragCoefficient = coefficient;
		_squaredDragCoefficient = squaredCoefficient;
	}

	virtual void Update(Particle* particle, real duration) {
		Vector3 totalDrag;
		totalDrag = particle->GetVelocity();

		//Calculate drag factor
		real dragFactor = totalDrag.Magnitude();
		dragFactor = _dragCoefficient * dragFactor + _squaredDragCoefficient * dragFactor * dragFactor;

		//Calculate total drag
		totalDrag.Normalize();
		totalDrag *= -dragFactor;

		//Add the drag to the particle
		particle->AddForce(totalDrag);
	}

	void SetDrags(real drag, real squaredDrag) {
		_dragCoefficient = drag;
		_squaredDragCoefficient = squaredDrag;
	}

	void SetDragCoefficient(real drag) {
		_dragCoefficient = drag;
	}

	void SetSquaredDragCoefficient(real drag) {
		_squaredDragCoefficient = drag;
	}
};
