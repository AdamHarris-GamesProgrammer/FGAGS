#include "ParticleContact.h"

void ParticleContact::Resolve(float dt)
{
	ResolveVelocity(dt);
	ResolveInterpenetration(dt);
}

float ParticleContact::CalculateSeperatingVelocity() const
{
	//Gets the velocity of particle A
	Vector3 relativeVelocity = _involvedParticles[0]->GetVelocity();

	//if particle B exists then we subtract Bs velocity from A
	if (_involvedParticles[1]) relativeVelocity -= _involvedParticles[1]->GetVelocity();

	//Return the calculated velocity based of the contact normal
	return relativeVelocity * _contactNormal;
}

void ParticleContact::ResolveVelocity(float dt)
{
	float seperatingVelocity = CalculateSeperatingVelocity();

	if (seperatingVelocity > 0) {
		//The contact is either separating or stationary, so no impulse required
		return;
	}

	float newSeperatingVelocity = -seperatingVelocity * _restitutionCoefficient;


	Vector3 accumulatedVelocity = _involvedParticles[0]->GetAcceleration();
	if (_involvedParticles[1]) accumulatedVelocity -= _involvedParticles[1]->GetAcceleration();

	float accumulatedSeperationVelocity = accumulatedVelocity * _contactNormal * dt;

	if (accumulatedSeperationVelocity < 0) {
		newSeperatingVelocity += _restitutionCoefficient * accumulatedSeperationVelocity;

		if (newSeperatingVelocity < 0) newSeperatingVelocity = 0;
	}


	float deltaVelocity = newSeperatingVelocity - seperatingVelocity;


	//To calculate an accurate repulsion force, the inverse mass of each object is needed
	float totalInverseMass = _involvedParticles[0]->GetInverseMass();
	if (_involvedParticles[1]) totalInverseMass += _involvedParticles[1]->GetInverseMass();

	//if the particles have infinite mass then a impulse is not required
	if(totalInverseMass <= 0) return;

	//Calculate the required impulse
	float impulse = deltaVelocity / totalInverseMass;

	//Calculate the amount of impulse needed per unit of mass
	Vector3 impulsePerMass = _contactNormal * impulse;

	//Calculate particle A's new velocity and set the velocity 
	_involvedParticles[0]->SetVelocity(_involvedParticles[0]->GetVelocity() + impulsePerMass * _involvedParticles[0]->GetInverseMass());

	if (_involvedParticles[1]) {
		//Particle B will go in the opposite direction as particle A
		_involvedParticles[1]->SetVelocity(_involvedParticles[1]->GetVelocity() + impulsePerMass * -_involvedParticles[1]->GetInverseMass());
	}



}

void ParticleContact::ResolveInterpenetration(float dt)
{
	//If there is no penetration then skip this step
	if (_penetrationDepth <= 0) return;

	//Calculates the total inverse mass of both particles
	float totalInverseMass = _involvedParticles[0]->GetInverseMass();
	if (_involvedParticles[1]) totalInverseMass += _involvedParticles[1]->GetInverseMass();

	//Objects have a infinite mass
	if (totalInverseMass <= 0) return;

	Vector3 movePerInverseMass = _contactNormal * (_penetrationDepth / totalInverseMass);

	_particleMovement[0] = movePerInverseMass * _involvedParticles[0]->GetInverseMass();
	if (_involvedParticles[1]) {
		_particleMovement[1] = movePerInverseMass * -_involvedParticles[1]->GetInverseMass();
	}
	else {
		_particleMovement[1].Zero();
	}

	_involvedParticles[0]->SetPosition(_involvedParticles[0]->GetPosition() + _particleMovement[0]);

	if (_involvedParticles[1]) {
		_involvedParticles[1]->SetPosition(_involvedParticles[1]->GetPosition() + _particleMovement[1]);
	}
	

}
