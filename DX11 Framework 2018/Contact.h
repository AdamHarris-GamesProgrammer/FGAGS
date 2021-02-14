#pragma once
#include "Vector3.h"
#include "ContactResolver.h"
#include "Rigidbody.h"
#include "Matrix3.h"

#ifndef Contact_H
#define Contact_H

class Contact {
	friend class ContactResolver;
public:
	Rigidbody* _bodies[2];

	float _friction;
	float _restitution;


	Vector3 _contactPoint;
	Vector3 _contactNormal;
	float _penetration;

	void SetBodyData(Rigidbody* a, Rigidbody* b, float friction, float restitution) {
		_bodies[0] = a;
		_bodies[1] = b;
		_friction = friction;
		_restitution = restitution;
	}

protected:
	Matrix3 _contactToWorld;
	Vector3 _contactVelocity;
	float _desiredDeltaVelocity;
	Vector3 _relativeContactPosition[2];

	void CalculateInternals(float dt);

	void SwapBodies();

	void MatchAwakeState();

	void CalculateDesiredDeltaVelocity(float dt);

	Vector3 CalculateLocalVelocity(unsigned bodyIndex, float dt);

	void CalculateContactBasis();

	void ApplyVelocityChange(Vector3 velocityChange[2], Vector3 rotationChange[2]);

	void ApplyPositionChange(Vector3 linearChange[2], Vector3 angularChange[2], float penetration);

	Vector3 CalculateFrictionlessImpulse(Matrix3* inverseIntertiaTensor);

	Vector3 CalculateFrictionImpulse(Matrix3* inverseInertiaTensor);

};

#endif



