#pragma once
#include "Vector3.h"
#include "Contact.h"

struct CollisionData {
	Contact* _contacts;
	Contact* _contactArray;

	int _contactsLeft;

	unsigned _contactCount;

	real _friction;
	real _restitution;
	real _tolerance;

	bool HasMoreContacts() {
		return _contactsLeft > 0;
	}

	void Reset(unsigned maxContacts) {
		_contactsLeft = maxContacts;
		_contactCount = 0;
		_contacts = _contactArray;
	}

	void AddContacts(unsigned count) {
		_contactsLeft -= count;
		_contactCount += count;

		_contacts += count;
	}
};
