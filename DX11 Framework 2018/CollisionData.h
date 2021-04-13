#pragma once
#include "Vector3.h"
#include "Contact.h"

struct CollisionData {
	//Holds an array of current contacts
	Contact* _contacts;
	Contact* _contactArray;

	//How many contacts are left, a way to optimize collision checking
	int _contactsLeft;

	//Stores the amount of contacts 
	unsigned _contactCount;

	//Holds the friction value that will be written into a collision
	float _friction;

	//Holds the restitution value that will be written into a collision
	float _restitution;

	//Tolerance value, any collisions with distance less than this will generate a contact.
	float _tolerance;

	/// <summary>
	/// Returns if more contacts are available
	/// </summary>
	bool HasMoreContacts() {
		return _contactsLeft > 0;
	}

	/// <summary>
	/// Resets the contact data struct
	/// </summary>
	/// <param name="maxContacts">The maxinum number of contacts in each frame</param>
	void Reset(unsigned maxContacts) {
		//Resets the contacts left to be our maxinum contacts
		_contactsLeft = maxContacts;
		//Sets our contact count 
		_contactCount = 0;
		//Resets our contacts array
		_contacts = _contactArray;
	}

	/// <summary>
	/// Adds a contact to the tracking system
	/// </summary>
	/// <param name="count"></param>
	void AddContacts(unsigned count) {
		_contactsLeft -= count;
		_contactCount += count;

		_contacts += count;
	}
};
