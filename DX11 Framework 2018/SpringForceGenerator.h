#pragma once
#include "Rigidbody.h"
#include "ForceGenerator.h"

class SpringForceGenerator : public ForceGenerator {
public:
	SpringForceGenerator(const Vector3& localConnectionPoint, Rigidbody* other,
		const Vector3& otherConnectionPoint, real springConstant, real restLength) {
		_connectionPoint = localConnectionPoint;
		_otherConnectionPoint = otherConnectionPoint;
		_other = other;
		_restLength = restLength;
		_springConstant = springConstant;
	}

	virtual void Update(Rigidbody* body, real dt) {
		//Local world space and Other World Space
		//Used to hold the points in space of the connections
		Vector3 lws = body->GetPointInWorldSpace(_connectionPoint);
		Vector3 ows = _other->GetPointInWorldSpace(_otherConnectionPoint);

		Vector3 force = lws - ows;

		real magnitude = force.Magnitude();
		magnitude = real_abs(magnitude - _restLength);
		magnitude *= _springConstant;

		
		force.Normalize();
		force *= -magnitude;
		body->AddForceAtPoint(force, lws);
	}

private:

	Vector3 _connectionPoint;

	Vector3 _otherConnectionPoint;

	Rigidbody* _other;

	real _springConstant;
	real _restLength;
};
