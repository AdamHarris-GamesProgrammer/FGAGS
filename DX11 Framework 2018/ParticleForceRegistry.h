#pragma once

#include "Precision.h"
#include "ParticleForceGenerator.h"
#include "Particle.h"
#include <algorithm>
#include <vector>

class ParticleForceRegistry
{
protected:
	struct ParticleForceRegistration
	{
		Particle* particle;
		ParticleForceGenerator* forceGenerator;
	};

	typedef std::vector<ParticleForceRegistration> Registry;
	Registry _registrations;

public:
	void Add(Particle* particle, ParticleForceGenerator* generator);

	void Clear();

	void Update(real dt);
};

