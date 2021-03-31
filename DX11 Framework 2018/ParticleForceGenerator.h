#pragma once

#include "Particle.h"

class ParticleForceGenerator {
public:
	virtual void Update(ParticleComponent* particle, float duration) = 0;
};