#pragma once
#include "Particle.h"

class ParticleBuoyancy {
public:


private:
	//The maximum submersion depth of the object
	float _maxDepth;

	//The volume of the object
	float _volume;

	//The height of the water plane above y = 0. The plane in this engine is parallel to the XZ plane
	float _waterHeight;

	//The liquid density, pure water has a density of 1000kg per cubic meter
	float _liquidDensity;

public:
	ParticleBuoyancy(float maxDepth, float volume, float waterHeight, float liquidDensity = 1000.0) {
		_maxDepth = maxDepth;
		_volume = volume;
		_waterHeight = waterHeight;
		_liquidDensity = liquidDensity;
	}

	virtual void Update(ParticleComponent* particle, float duration) {
		//Calculate the submersion depth
		float submersionDepth = particle->GetPosition().y;

		//if the submersion depth is greater than the water height + the max depth then our object is out of water
		if (submersionDepth >= _waterHeight) return;

		Vector3 force = Vector3();

		//We are fully submerged
		if (submersionDepth <= _waterHeight) {
			force.y = _liquidDensity * _volume;
			particle->AddForce(force);
			return;
		}

		//We are partly submerged
		force.y = _liquidDensity * _volume * 
			(submersionDepth - _maxDepth - _waterHeight) / (2 * _maxDepth);
		particle->AddForce(force);
	}
};
