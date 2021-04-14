#include "ContactResolver.h"

//Constructors
ContactResolver::ContactResolver(unsigned iterations, float velocityEps /*= (float)0.01*/, float positionEps /*= (float)0.01*/)
	: _positionIterations(iterations), _velocityIterations(iterations), _positionEpsilon(positionEps), _velocityEpsilon(velocityEps) {}

ContactResolver::ContactResolver(unsigned velocityIt, unsigned positionIt, float positionEp /*= (float)0.01*/, float velocityEp /*= (float)0.01*/)
	: _positionIterations(positionIt), _velocityIterations(velocityIt), _velocityEpsilon(velocityEp), _positionEpsilon(positionEp) {}



void ContactResolver::ResolveContacts(Contact* contacts, unsigned numContacts, float dt)
{
	//Checks we have contacts
	if (numContacts == 0) return;

	//Prepare the contacts
	PrepareContacts(contacts, numContacts, dt);

	//Adjust positions, remove penetration
	AdjustPositions(contacts, numContacts, dt);

	//Adjust velocities
	AdjustVelocities(contacts, numContacts, dt);
}

void ContactResolver::AdjustVelocities(Contact* contacts, unsigned numContacts, float dt)
{
	Vector3 velocityChange[2], rotationChange[2];
	Vector3 deltaVel;

	//Sets this value back to 0
	_velocityIterationsUsed = 0;

	//loops until we have reached our max iterations
	while (_velocityIterationsUsed < _velocityIterations)
	{
		//Find the contact with the maximum magnitude of velocity change
		float max = _velocityEpsilon;
		unsigned index = numContacts;
		for (unsigned i = 0; i < numContacts; i++) {
			//if the current delta velocity is greater than our max
			if (contacts[i]._desiredDeltaVelocity > max) {
				//set our new max value
				max = contacts[i]._desiredDeltaVelocity;
				index = i;
			}
		}

		//if our index is still equal to the number of contacts that means none of the contacts are greater than our epsilon value
		if (index == numContacts) break;

		//Matches awake state for the Rigidbodies
		contacts[index].MatchAwakeState();

		//Applies the velocity change 
		contacts[index].ApplyVelocityChange(velocityChange, rotationChange);

		//Now that the velocity has changed in the two bodies some of the closing velocities need readjusting
		for (unsigned i = 0; i < numContacts; i++)
		{
			// Cycle through each body in the contact
			for (unsigned b = 0; b < 2; b++) if (contacts[i]._bodies[b])
			{
				// Check for a match with each body in the new contact
				for (unsigned d = 0; d < 2; d++)
				{
					if (contacts[i]._bodies[b] == contacts[index]._bodies[d])
					{
						//Calculate our delta velocity
						deltaVel = velocityChange[d] + rotationChange[d].VectorProduct(contacts[i]._relativeContactPosition[b]);

						// The sign of the change is negative if we're dealing with the second body in a contact.
						int sign = (b ? -1 : 1);
						//Add the contact contract to the contact velocity
						contacts[i]._contactVelocity += contacts[i]._contactToWorld.TransformTranspose(deltaVel) * sign;
						//Calculate our new desired velocity
						contacts[i].CalculateDesiredDeltaVelocity(dt);
					}
				}
			}
		}
		//increment our iterations used variable
		_velocityIterationsUsed++;
	}
}

void ContactResolver::AdjustPositions(Contact* contacts, unsigned numContacts, float dt)
{
	Vector3 linearChange[2], angularChange[2];
	
	//reset our iterations variable
	_positionIterationsUsed = 0;

	//loop until we have run out of iterations
	while (_positionIterationsUsed < _positionIterations) {

		//Find the biggest penetration
		float max = _positionEpsilon;
		unsigned index = numContacts;
		for (unsigned i = 0; i < numContacts; i++) {
			if (contacts[i]._penetration > max) {
				max = contacts[i]._penetration;
				index = i;
			}
		}

		//if index is equal to the number of contacts then none of our contacts had a inter-penetration above our epsilon
		if (index == numContacts) break;

		//Match the awake states of our rigidbodies
		contacts[index].MatchAwakeState();

		//Apply position changes to the bodies
		contacts[index].ApplyPositionChange(linearChange, angularChange, max);

		//cycle through each contact
		for (unsigned i = 0; i < numContacts; i++)
		{
			//cycle through each body in the contact
			for (unsigned b = 0; b < 2; b++) if (contacts[i]._bodies[b])
			{
				// Check for a match with each _bodies in the new contact
				for (unsigned d = 0; d < 2; d++)
				{
					if (contacts[i]._bodies[b] == contacts[index]._bodies[d])
					{
						//Calculate our change in position this frame 
						Vector3 deltaPosition = linearChange[d] + angularChange[d].VectorProduct(contacts[i]._relativeContactPosition[b]);

						//The sign needs to be negative if we are dealing with the second body of the contact
						int sign = (b ? 1 : -1);
						contacts[i]._penetration += deltaPosition.ScalarProduct(contacts[i]._contactNormal) * sign;
					}
				}
			}
		}
		_positionIterationsUsed++;
	}
}

void ContactResolver::PrepareContacts(Contact* contacts, unsigned numContacts, float dt)
{
	//uses pointer arithmetic to determine the last contact in the array
	Contact* lastContact = contacts + numContacts;

	//cycles through each one and calculates there internals
	for (Contact* contact = contacts; contact < lastContact; contact++) {
		contact->CalculateInternals(dt);
	}
}
