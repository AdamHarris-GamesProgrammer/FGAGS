#pragma once

#include "Particle.h"
#include "Precision.h"

class ParticleForceGenerator {
public:
	virtual void Update(Particle* particle, real duration) = 0;
};