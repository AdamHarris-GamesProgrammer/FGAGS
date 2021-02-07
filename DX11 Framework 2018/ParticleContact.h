#pragma once
#include "Particle.h"

class ParticleContact
{
	friend class ParticleContactResolver;

public:
	//Holds the particles that are involved in this collision
	Particle* _involvedParticles[2];

	//Holds the normal restitution coefficient at the point of contact
	real _restitutionCoefficient;

	//Holds the direction of the contact in World Space
	Vector3 _contactNormal;

	//Holds the movement of both particles
	Vector3 _particleMovement[2];

	//Holds the depth of penetration
	real _penetrationDepth;
	
protected:
	//Resolves the contact for both the velocity and inter penetration
	void Resolve(real dt);

	//Calculates the separating velocity
	real CalculateSeperatingVelocity() const;

private:
	//Calculates impulses for this collision
	void ResolveVelocity(real dt);

	void ResolveInterpenetration(real dt);
};

