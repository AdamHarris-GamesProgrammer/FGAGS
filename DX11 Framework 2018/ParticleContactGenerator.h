#pragma once
#include "ParticleContact.h"

class ParticleContactGenerator {
public:
	virtual unsigned AddContact(ParticleContact* contact, unsigned limit) const = 0;
};
