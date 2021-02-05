#pragma once
#include "Precision.h"
#include "Particle.h"
#include "ParticleForceGenerator.h"

class ParticleBuoyancy : public ParticleForceGenerator {
public:


private:
	//The maxinum submersion depth of the object
	real _maxDepth;

	//The volume of the object
	real _volume;

	//The height of the water plane above y = 0. The plane in this engine is parallel to the XZ plane
	real _waterHeight;

	//The liquid density, pure water has a density of 1000kg per cubic meter
	real _liquidDensity;

public:
	ParticleBuoyancy(real maxDepth, real volume, real waterHeight, real liquidDensity = 1000.0) {
		_maxDepth = maxDepth;
		_volume = volume;
		_waterHeight = waterHeight;
		_liquidDensity = liquidDensity;
	}

	virtual void Update(Particle* particle, real duration) {
		//Calculate the submersion depth
		real submersionDepth = particle->GetPosition().y;

		//if the submersion depth is greater than the water height + the max depth then our object is out of water
		if (submersionDepth >= _waterHeight + _maxDepth) return;

		Vector3 force = Vector3();

		//We are fully submerged
		if (submersionDepth <= _waterHeight - _maxDepth) {
			force.y = _liquidDensity * _volume;
			particle->AddForce(force);
			return;
		}

		//We are partly submerged
		force.y = _liquidDensity * _volume * 
			(submersionDepth - _maxDepth - _waterHeight) / 2 * _maxDepth;
		particle->AddForce(force);
	}
};
