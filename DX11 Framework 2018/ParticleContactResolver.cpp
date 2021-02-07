#include "ParticleContactResolver.h"

ParticleContactResolver::ParticleContactResolver(unsigned iterations)
{
	_iterations = iterations;
}

void ParticleContactResolver::ResolveContacts(ParticleContact* contactsArray, unsigned numContacts, real dt)
{
	unsigned i;

	_iterationsUsed = 0;

	while (_iterationsUsed < _iterations) {

		
		real max = REAL_MAX;
		unsigned maxIndex = numContacts;

		//Find the contact with the largest closing velocity
		for (i = 0; i < numContacts; i++) {
			real seperationVelocity = contactsArray[i].CalculateSeperatingVelocity();
			if (seperationVelocity < max && (seperationVelocity < 0 || contactsArray[i]._penetrationDepth > 0)) {
				max = seperationVelocity;
				maxIndex = i;
			}
		}

		//If we do not have anything worth processing then break
		if (maxIndex == numContacts) break;

		contactsArray[maxIndex].Resolve(dt);

		_iterationsUsed++;
	}
}
