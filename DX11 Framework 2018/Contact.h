#pragma once
#include "Vector3.h"
#include "ContactResolver.h"
#include "Rigidbody.h"
#include "Matrix3.h"

#ifndef Contact_H
#define Contact_H

class Contact {
	//Contact resolver can see this classes private members.
	friend class ContactResolver;
public:
	//Store the two Rigidbodies involved in the collision
	RigidbodyComponent* _bodies[2] = { nullptr, nullptr };

	//The friction and restitution factors for this collision, passed in through collision data array
	float _friction;
	float _restitution;

	//Stores the point of contact for the collision
	Vector3 _contactPoint;

	//Stores the normal for the collision
	Vector3 _contactNormal;

	//Stores the penetration value of the collision
	float _penetration;

	/// <summary>
	/// Sets the data for a contact
	/// </summary>
	/// <param name="a">The first colliders rigidbody</param>
	/// <param name="b">The second colliders rigidbody</param>
	/// <param name="friction">The friction coefficient of this collision</param>
	/// <param name="restitution">The restitution coefficient of this collision</param>
	void SetBodyData(RigidbodyComponent* a, RigidbodyComponent* b, float friction, float restitution) {
		_bodies[0] = a;
		_bodies[1] = b;
		_friction = friction;
		_restitution = restitution;
	}

protected:
	//A transform matrix that converts the contacts coordinates into world space
	Matrix3 _contactToWorld;

	//Holds the velocity of the collision
	Vector3 _contactVelocity;

	//This is the desired amount of velocity for the collision to be resolved
	float _desiredDeltaVelocity;

	//World space position of the contact point relative to the center of each body.
	Vector3 _relativeContactPosition[2];

	//This calculates the state data
	void CalculateInternals(float dt);

	//Reverses the contacts, swaps the pointers around, this is needed for collision objects which don't have a rigidbody (plane)
	void SwapBodies();

	//Updates the awake state of the Rigidbodies, it will make a rigidbody awake if it is in contact with a body that is awake
	void MatchAwakeState();

	//Calculates the internal value for the desired velocity based off delta time
	void CalculateDesiredDeltaVelocity(float dt);

	//Calculates the velocity of the contact point on the provided body.
	Vector3 CalculateLocalVelocity(unsigned bodyIndex, float dt);

	//Calculates a basis for the contact point 
	void CalculateContactBasis();


	//performs a inertia based velocity change on this contact
	void ApplyVelocityChange(Vector3 velocityChange[2], Vector3 rotationChange[2]);

	//Performs a inertia based position change on this contact
	void ApplyPositionChange(Vector3 linearChange[2], Vector3 angularChange[2], float penetration);

	//Calculates the amount of impulse needed to resolve this contact if there is no friction
	Vector3 CalculateFrictionlessImpulse(Matrix3* inverseIntertiaTensor);


	//Calculate the amount of impulse needed to resolve this contact with friction
	Vector3 CalculateFrictionImpulse(Matrix3* inverseInertiaTensor);

};

#endif



