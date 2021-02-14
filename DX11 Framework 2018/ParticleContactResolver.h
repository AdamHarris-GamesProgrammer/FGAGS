#pragma once
#include "ParticleContact.h"

class ParticleContactResolver
{
public:
	ParticleContactResolver(unsigned iterations);

	void SetIterations(unsigned iterations) { _iterations = iterations; }

	void ResolveContacts(ParticleContact* contactsArray, unsigned numContacts, float dt);

protected:
	//Holds the number of iterations allowed per frame
	unsigned _iterations;

	//Holds the amount fo actual iterations used
	unsigned _iterationsUsed;

private:

};

