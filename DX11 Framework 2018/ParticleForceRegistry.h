#pragma once

#include "ParticleForceGenerator.h"
#include "Particle.h"
#include <algorithm>
#include <vector>

class ParticleForceRegistry
{
protected:
	struct ParticleForceRegistration
	{
		ParticleComponent* particle;
		ParticleForceGenerator* forceGenerator;
	};

	typedef std::vector<ParticleForceRegistration> Registry;
	Registry _registrations;

public:
	void Add(ParticleComponent* particle, ParticleForceGenerator* generator);

	void Clear();

	void Update(float dt);
};

