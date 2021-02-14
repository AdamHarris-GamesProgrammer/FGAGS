#pragma once
#include "ParticleLink.h"

class ParticleRod : public ParticleLink {
public:
	float _length;

	virtual unsigned AddContact(ParticleContact* contact, unsigned limit) const {
		float currentLength = CurrentLength();

		if (currentLength == _length) {
			return 0;
		}

		contact->_involvedParticles[0] = _particles[0];
		contact->_involvedParticles[1] = _particles[1];

		Vector3 normal = _particles[1]->GetPosition() - _particles[0]->GetPosition();
		normal.Normalize();

		if (currentLength > _length) {
			contact->_contactNormal = normal;
			contact->_penetrationDepth = currentLength - _length;
		}
		else {
			contact->_contactNormal = normal * -1;
			contact->_penetrationDepth = _length - currentLength;
		}

		contact->_restitutionCoefficient = 0;

		return 1;
	}
};