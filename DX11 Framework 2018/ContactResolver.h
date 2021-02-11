#pragma once
#include "Contact.h"
#include "Precision.h"


#ifndef ContactResolver_H
#define ContactResolver_H

class ContactResolver {
	friend class Contact;
protected:
	unsigned velocityIterations;
	unsigned positionIterations;

	real velocityEpsilon = 0.01;
	real positionEpsilon = 0.01;

public:
	unsigned velocityIterationsUsed;
	unsigned positionIterationsUsed;

private:
	bool validSettings;

public:
	ContactResolver(unsigned iterations, real velocityEps = (real)0.01, real positionEps = (real)0.01);
	ContactResolver(unsigned velocityIt, unsigned positionIt, real positionEp = (real)0.01, real velocityEp = (real)0.01);

	bool IsValid();

	void SetIterations(unsigned velocityIt, unsigned positionIt);

	void SetIterations(unsigned iterations);

	void SetEpsilon(real velocityEp, real positionEp);

	void ResolveContacts(Contact* contacts, unsigned numContacts, real dt);


protected:

	void AdjustVelocities(Contact* contacts, unsigned numContacts, real dt);



	void AdjustPositions(Contact* contacts, unsigned numContacts, real dt);



	void PrepareContacts(Contact* contacts, unsigned numContacts, real dt);


};

#endif

