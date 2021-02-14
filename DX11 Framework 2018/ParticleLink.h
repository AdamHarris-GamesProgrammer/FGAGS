#pragma once
#include "Particle.h"
#include "Precision.h"
#include "ParticleContactGenerator.h"

class ParticleLink : public ParticleContactGenerator {
public:
	Particle* _particles[2];

	virtual unsigned AddContact(ParticleContact* contact, unsigned limit) const = 0;

protected:
	//Current Length of the link
	float CurrentLength() const {
		//Works out the direction vector
		Vector3 relativePos = _particles[0]->GetPosition() - _particles[1]->GetPosition();

		//Returns the magnitude
		return relativePos.Magnitude();
	}
};