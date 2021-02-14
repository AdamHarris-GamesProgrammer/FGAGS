#pragma once
#include "ParticleLink.h"

class ParticleCable : public ParticleLink {
public:
	float _maxLength;

	float _restitution;

	virtual unsigned AddContact(ParticleContact* contact, unsigned limit) const {
		float length = CurrentLength();

		if (length < _maxLength) {
			return 0;
		}

		contact->_involvedParticles[0] = _particles[0];
		contact->_involvedParticles[1] = _particles[1];

		Vector3 normal = _particles[1]->GetPosition() - _particles[0]->GetPosition();
		normal.Normalize();

		contact->_contactNormal = normal;
		contact->_penetrationDepth = length - _maxLength;
		contact->_restitutionCoefficient = _restitution;

		return 1;
	}
};
