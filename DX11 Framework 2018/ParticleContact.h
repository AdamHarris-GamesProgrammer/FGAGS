#pragma once
#include "Particle.h"

class ParticleContact
{
	friend class ParticleContactResolver;

public:
	//Holds the particles that are involved in this collision
	ParticleComponent* _involvedParticles[2];

	//Holds the normal restitution coefficient at the point of contact
	float _restitutionCoefficient;

	//Holds the direction of the contact in World Space
	Vector3 _contactNormal;

	//Holds the movement of both particles
	Vector3 _particleMovement[2];

	//Holds the depth of penetration
	float _penetrationDepth;
	
protected:
	//Resolves the contact for both the velocity and inter penetration
	void Resolve(float dt);

	//Calculates the separating velocity
	float CalculateSeperatingVelocity() const;

private:
	//Calculates impulses for this collision
	void ResolveVelocity(float dt);

	void ResolveInterpenetration(float dt);
};

