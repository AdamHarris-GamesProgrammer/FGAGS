#include "Contact.h"

void Contact::CalculateInternals(float dt)
{
	if (!_bodies[0]) SwapBodies();
	assert(_bodies[0]);

	CalculateContactBasis();

	_relativeContactPosition[0] = _contactPoint - _bodies[0]->GetPosition();
	if (_bodies[1]) {
		_relativeContactPosition[1] = _contactPoint - _bodies[1]->GetPosition();
	}

	_contactVelocity = CalculateLocalVelocity(0, dt);
	if (_bodies[1]) {
		_contactVelocity -= CalculateLocalVelocity(1, dt);
	}


	CalculateDesiredDeltaVelocity(dt);
}

void Contact::SwapBodies()
{
	_contactNormal *= -1;

	Rigidbody* temp = _bodies[0];
	_bodies[0] = _bodies[1];
	_bodies[1] = temp;
}

void Contact::MatchAwakeState()
{
	if (!_bodies[1]) return;

	bool bodyA = _bodies[0]->GetAwake();
	bool bodyB = _bodies[1]->GetAwake();

	if (bodyA ^ bodyB) {
		if (bodyA) _bodies[1]->SetAwake();
		else _bodies[0]->SetAwake();
	}
}

void Contact::CalculateDesiredDeltaVelocity(float dt)
{
	const static float velocityLimit = (float)0.25f;

	float velocityFromAcc = 0;

	if (_bodies[0]->GetAwake()) {
		velocityFromAcc += _bodies[0]->GetLastFrameAcceleration() * dt * _contactNormal;
	}

	if (_bodies[1] && _bodies[1]->GetAwake()) {
		velocityFromAcc -= _bodies[1]->GetLastFrameAcceleration() * dt * _contactNormal;
	}

	float thisRestitution = _restitution;
	if (fabsf(_contactVelocity.x) < velocityLimit) {
		thisRestitution = (float)0.0f;
	}

	_desiredDeltaVelocity = -_contactVelocity.x - thisRestitution * (_contactVelocity.x - velocityFromAcc);
}

Vector3 Contact::CalculateLocalVelocity(unsigned bodyIndex, float dt)
{
	Rigidbody* thisBody = _bodies[bodyIndex];

	Vector3 velocity = thisBody->GetRotation() % _relativeContactPosition[bodyIndex];
	velocity += thisBody->GetVelocity();

	Vector3 contactVelocity = _contactToWorld.TransformTranspose(velocity);

	Vector3 accVelocity = thisBody->GetLastFrameAcceleration() * dt;

	accVelocity = _contactToWorld.TransformTranspose(accVelocity);

	accVelocity.x = 0;

	_contactVelocity += accVelocity;

	return contactVelocity;
}

void Contact::CalculateContactBasis()
{
	Vector3 contactTangent[2];

	if (fabsf(_contactNormal.x) > fabsf(_contactNormal.y)) {
		//Scaling factor
		const float s = (float)1.0f / sqrtf(_contactNormal.x * _contactNormal.x);

		//The new x-axis is at right angles to the world Y-axis
		contactTangent[0].x = _contactNormal.z * s;
		contactTangent[0].y = 0;
		contactTangent[0].z = -_contactNormal.x * s;

		contactTangent[1].x = _contactNormal.y * contactTangent[0].x;
		contactTangent[1].y = _contactNormal.z * contactTangent[0].x - _contactNormal.x * contactTangent[0].z;
		contactTangent[1].z = -_contactNormal.y * contactTangent[0].x;
	}
	else {
		const float s = (float)1.0 / sqrtf(_contactNormal.z * _contactNormal.z + _contactNormal.y * _contactNormal.y);

		contactTangent[0].x = 0;
		contactTangent[0].y = -_contactNormal.z * s;
		contactTangent[0].z = _contactNormal.y * s;

		contactTangent[1].x = _contactNormal.y * contactTangent[0].z - _contactNormal.z * contactTangent[0].y;
		contactTangent[1].y = -_contactNormal.x * contactTangent[0].z;
		contactTangent[1].z = _contactNormal.x * contactTangent[0].y;
	}

	_contactToWorld.SetComponents(_contactNormal, contactTangent[0], contactTangent[1]);
}

void Contact::ApplyVelocityChange(Vector3 velocityChange[2], Vector3 rotationChange[2])
{
	// Get hold of the inverse mass and inverse inertia tensor, both in
// world coordinates.
	Matrix3 inverseInertiaTensor[2];
	_bodies[0]->GetInverseInertiaTensorWorld(&inverseInertiaTensor[0]);
	if (_bodies[1])
		_bodies[1]->GetInverseInertiaTensorWorld(&inverseInertiaTensor[1]);

	// We will calculate the impulse for each contact axis
	Vector3 impulseContact;

	if (_friction == (float)0.0)
	{
		// Use the short format for frictionless contacts
		impulseContact = CalculateFrictionlessImpulse(inverseInertiaTensor);
	}
	else
	{
		// Otherwise we may have impulses that aren't in the direction of the
		// contact, so we need the more complex version.
		impulseContact = CalculateFrictionImpulse(inverseInertiaTensor);
	}

	// Convert impulse to world coordinates
	Vector3 impulse = _contactToWorld.Transform(impulseContact);

	// Split in the impulse into linear and rotational components
	Vector3 impulsiveTorque = _relativeContactPosition[0] % impulse;
	rotationChange[0] = inverseInertiaTensor[0].Transform(impulsiveTorque);
	velocityChange[0].Zero();
	velocityChange[0].AddScaledVector(impulse, _bodies[0]->GetInverseMass());

	// Apply the changes
	_bodies[0]->AddVelocity(velocityChange[0]);
	_bodies[0]->AddRotation(rotationChange[0]);

	if (_bodies[1])
	{
		// Work out _bodies one's linear and angular changes
		Vector3 impulsiveTorque = impulse % _relativeContactPosition[1];
		rotationChange[1] = inverseInertiaTensor[1].Transform(impulsiveTorque);
		velocityChange[1].Zero();
		velocityChange[1].AddScaledVector(impulse, -_bodies[1]->GetInverseMass());

		// And apply them.
		_bodies[1]->AddVelocity(velocityChange[1]);
		_bodies[1]->AddRotation(rotationChange[1]);
	}
}

void Contact::ApplyPositionChange(Vector3 linearChange[2], Vector3 angularChange[2], float penetration)
{
	const float angularLimit = (float)0.2f;
	float angularMove[2];
	float linearMove[2];

	float totalInertia = 0;
	float linearInertia[2];
	float angularInertia[2];

	// We need to work out the inertia of each object in the direction
	// of the contact normal, due to angular inertia only.
	for (unsigned i = 0; i < 2; i++) if (_bodies[i])
	{
		Matrix3 inverseInertiaTensor;
		_bodies[i]->GetInverseInertiaTensorWorld(&inverseInertiaTensor);

		// Use the same procedure as for calculating frictionless
		// velocity change to work out the angular inertia.
		Vector3 angularInertiaWorld =
			_relativeContactPosition[i] % _contactNormal;
		angularInertiaWorld =
			inverseInertiaTensor.Transform(angularInertiaWorld);
		angularInertiaWorld =
			angularInertiaWorld % _relativeContactPosition[i];
		angularInertia[i] =
			angularInertiaWorld * _contactNormal;

		// The linear component is simply the inverse mass
		linearInertia[i] = _bodies[i]->GetInverseMass();

		// Keep track of the total inertia from all components
		totalInertia += linearInertia[i] + angularInertia[i];
	}

	for (unsigned i = 0; i < 2; i++) if (_bodies[i]) {
		float sign = (i == 0) ? 1 : -1;
		angularMove[i] = sign * penetration * (angularInertia[i] / totalInertia);
		linearMove[i] = sign * penetration * (linearInertia[i] / totalInertia);

		Vector3 projection = _relativeContactPosition[i];
		projection.AddScaledVector(_contactNormal, -_relativeContactPosition[i].ScalarProduct(_contactNormal));

		float maxMagnitude = angularLimit * projection.Magnitude();

		if (angularMove[i] < -maxMagnitude) {
			float totalMove = angularMove[i] + linearMove[i];
			angularMove[i] = -maxMagnitude;
			linearMove[i] = totalMove - angularMove[i];
		}
		else if (angularMove[i] > maxMagnitude) {
			float totalMove = angularMove[i] + linearMove[i];
			angularMove[i] = maxMagnitude;
			linearMove[i] = totalMove - angularMove[i];
		}

		if (angularMove[i] == 0) {
			angularChange[i].Zero();
		}
		else {
			Vector3 targetAngularDirection = _relativeContactPosition[i].VectorProduct(_contactNormal);

			Matrix3 inverseInertiaTensor;
			_bodies[i]->GetInverseInertiaTensorWorld(&inverseInertiaTensor);

			angularChange[i] = inverseInertiaTensor.Transform(targetAngularDirection) * (angularMove[i] / angularInertia[i]);
		}

		linearChange[i] = _contactNormal * linearMove[i];

		Vector3 pos;

		pos = _bodies[i]->GetPosition();
		pos.AddScaledVector(_contactNormal, linearMove[i]);
		_bodies[i]->SetPosition(pos);

		Quaternion q;
		_bodies[i]->GetOrientation(&q);
		q.AddScaledVector(angularChange[i], ((float)1.0));
		_bodies[i]->SetOrientation(q);

		if (!_bodies[i]->GetAwake()) _bodies[i]->CalculateDerivedData();
	}
}

Vector3 Contact::CalculateFrictionlessImpulse(Matrix3* inverseIntertiaTensor)
{
	Vector3 impulseContact;

	Vector3 deltaVelWorld = _relativeContactPosition[0] % _contactNormal;
	deltaVelWorld = inverseIntertiaTensor[0].Transform(deltaVelWorld);
	deltaVelWorld = deltaVelWorld % _relativeContactPosition[0];

	float deltaVelocity = deltaVelWorld * _contactNormal;

	deltaVelocity += _bodies[0]->GetInverseMass();

	if (_bodies[1]) {
		Vector3 deltaVelWorld = _relativeContactPosition[1] % _contactNormal;
		deltaVelWorld = inverseIntertiaTensor[1].Transform(deltaVelWorld);
		deltaVelWorld = deltaVelWorld % _relativeContactPosition[1];

		deltaVelocity += deltaVelWorld * _contactNormal;

		deltaVelocity += _bodies[1]->GetInverseMass();
	}

	impulseContact.x = _desiredDeltaVelocity / deltaVelocity;
	impulseContact.y = 0;
	impulseContact.z = 0;
	return impulseContact;
}

Vector3 Contact::CalculateFrictionImpulse(Matrix3* inverseInertiaTensor)
{
	Vector3 impulseContact;
	float inverseMass = _bodies[0]->GetInverseMass();

	// The equivalent of a cross product in matrices is multiplication
	// by a skew symmetric matrix - we build the matrix for converting
	// between linear and angular quantities.
	Matrix3 impulseToTorque;
	impulseToTorque.SetSkewSymmetric(_relativeContactPosition[0]);

	// Build the matrix to convert contact impulse to change in velocity
	// in world coordinates.
	Matrix3 deltaVelWorld = impulseToTorque;
	deltaVelWorld *= inverseInertiaTensor[0];
	deltaVelWorld *= impulseToTorque;
	deltaVelWorld *= -1;

	// Check if we need to add _bodies two's _data
	if (_bodies[1])
	{
		// Set the cross product matrix
		impulseToTorque.SetSkewSymmetric(_relativeContactPosition[1]);

		// Calculate the velocity change matrix
		Matrix3 deltaVelWorld2 = impulseToTorque;
		deltaVelWorld2 *= inverseInertiaTensor[1];
		deltaVelWorld2 *= impulseToTorque;
		deltaVelWorld2 *= -1;

		// Add to the total delta velocity.
		deltaVelWorld += deltaVelWorld2;

		// Add to the inverse mass
		inverseMass += _bodies[1]->GetInverseMass();
	}

	// Do a change of basis to convert into contact coordinates.
	Matrix3 deltaVelocity = _contactToWorld.Transpose();
	deltaVelocity *= deltaVelWorld;
	deltaVelocity *= _contactToWorld;

	// Add in the linear velocity change
	deltaVelocity._data[0] += inverseMass;
	deltaVelocity._data[4] += inverseMass;
	deltaVelocity._data[8] += inverseMass;

	// Invert to get the impulse needed per unit velocity
	Matrix3 impulseMatrix = deltaVelocity.Inverse();

	// Find the target velocities to kill
	Vector3 velKill(_desiredDeltaVelocity,
		-_contactVelocity.y,
		-_contactVelocity.z);

	// Find the impulse to kill target velocities
	impulseContact = impulseMatrix.Transform(velKill);

	// Check for exceeding friction
	float planarImpulse = sqrtf(
		impulseContact.y * impulseContact.y +
		impulseContact.z * impulseContact.z
	);
	if (planarImpulse > impulseContact.x * _friction)
	{
		// We need to use dynamic friction
		impulseContact.y /= planarImpulse;
		impulseContact.z /= planarImpulse;

		impulseContact.x = deltaVelocity._data[0] +
			deltaVelocity._data[1] * _friction * impulseContact.y +
			deltaVelocity._data[2] * _friction * impulseContact.z;
		impulseContact.x = _desiredDeltaVelocity / impulseContact.x;
		impulseContact.y *= _friction * impulseContact.x;
		impulseContact.z *= _friction * impulseContact.x;
	}
	return impulseContact;
}

