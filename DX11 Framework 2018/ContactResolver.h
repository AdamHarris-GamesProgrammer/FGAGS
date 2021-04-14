#pragma once
#include "Contact.h"


#ifndef ContactResolver_H
#define ContactResolver_H

class ContactResolver {
	//Contact can access this classes private members
	friend class Contact;
protected:
	//Stores the amount of maximum iterations on the velocity and position
	unsigned _velocityIterations;
	unsigned _positionIterations;

	//used for stabilization of larger bodies when colliding with each other
	float _velocityEpsilon = 0.01f;
	float _positionEpsilon = 0.01f;

public:
	//Stores the amount of iterations used
	unsigned _velocityIterationsUsed;
	unsigned _positionIterationsUsed;

public:
	//Constructors with a series of default values
	ContactResolver(unsigned iterations, float velocityEps = 0.01f, float positionEps = 0.01f);
	ContactResolver(unsigned velocityIt, unsigned positionIt, float positionEp = 0.01f, float velocityEp = 0.01f);

	//Process for resolving contacts
	void ResolveContacts(Contact* contacts, unsigned numContacts, float dt);

protected:
	//Adjusts the velocities of the objects
	void AdjustVelocities(Contact* contacts, unsigned numContacts, float dt);

	//Adjusts the positions
	void AdjustPositions(Contact* contacts, unsigned numContacts, float dt);

	//Prepares the contacts of the objects
	void PrepareContacts(Contact* contacts, unsigned numContacts, float dt);
};

#endif

