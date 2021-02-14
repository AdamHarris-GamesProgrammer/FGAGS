#pragma once

#include "Particle.h"

class ParticleForceGenerator {
public:
	virtual void Update(Particle* particle, float duration) = 0;
};