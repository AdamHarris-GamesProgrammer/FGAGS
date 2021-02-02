#include "ParticleForceRegistry.h"

void ParticleForceRegistry::Add(Particle* particle, ParticleForceGenerator* generator)
{
	ParticleForceRegistration newForce;
	newForce.particle = particle;
	newForce.forceGenerator = generator;

	_registrations.push_back(newForce);
}



void ParticleForceRegistry::Clear()
{
	_registrations.clear();
}

void ParticleForceRegistry::Update(real dt)
{
	//Sets a iterator at the start of the vector
	Registry::iterator i = _registrations.begin();

	//Loops through registry and updates all particles
	for (; i != _registrations.end(); i++) {
		i->forceGenerator->Update(i->particle, dt);
	}
}
