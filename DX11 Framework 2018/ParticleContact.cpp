#include "ParticleContact.h"

void ParticleContact::Resolve(real dt)
{
	ResolveVelocity(dt);
	ResolveInterpenetration(dt);
}

real ParticleContact::CalculateSeperatingVelocity() const
{
	//Gets the velocity of particle A
	Vector3 relativeVelocity = _involvedParticles[0]->GetVelocity();

	//if particle B exists then we subtract Bs velocity from A
	if (_involvedParticles[1]) relativeVelocity -= _involvedParticles[1]->GetVelocity();

	//Return the calculated velocity based of the contact normal
	return relativeVelocity * _contactNormal;
}

void ParticleContact::ResolveVelocity(real dt)
{
	real seperatingVelocity = CalculateSeperatingVelocity();

	if (seperatingVelocity > 0) {
		//The contact is either seperating or stationary, so no impulse required
		return;
	}

	real newSeperatingVelocity = -seperatingVelocity * _restitutionCoefficient;

	real deltaVelocity = newSeperatingVelocity - seperatingVelocity;


	//To calculate an accurate repulsion force, the inverse mass of each object is needed
	real totalInverseMass = _involvedParticles[0]->GetInverseMass();
	if (_involvedParticles[1]) totalInverseMass += _involvedParticles[1]->GetInverseMass();

	//if the particles have infinite mass then a impulse is not required
	if(totalInverseMass <= 0) return;

	//Calculate the required impulse
	real impulse = deltaVelocity / totalInverseMass;

	//Calculate the amount of impulse needed per unit of mass
	Vector3 impulsePerMass = _contactNormal * impulse;

	_involvedParticles[0]->SetVelocity(_involvedParticles[0]->GetVelocity() + impulsePerMass * _involvedParticles[0]->GetInverseMass());

	if (_involvedParticles[1]) {
		_involvedParticles[1]->SetVelocity(_involvedParticles[1]->GetVelocity() + impulsePerMass * -_involvedParticles[1]->GetInverseMass());
	}



}

void ParticleContact::ResolveInterpenetration(real dt)
{
	
}
