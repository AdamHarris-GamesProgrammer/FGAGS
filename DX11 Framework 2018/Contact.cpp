#include "Contact.h"

void Contact::CalculateInternals(float dt)
{
	//Checks we have a rigidbody at the first element of the array
	if (!_bodies[0]) SwapBodies();
	assert(_bodies[0]);

	//Calculates our contact basis
	CalculateContactBasis();

	//Stores the relative position of each contact relative to their body
	_relativeContactPosition[0] = _contactPoint - _bodies[0]->GetPosition();
	if (_bodies[1]) {
		_relativeContactPosition[1] = _contactPoint - _bodies[1]->GetPosition();
	}

	//stores the relative velocity of the bodies at the contact point
	_contactVelocity = CalculateLocalVelocity(0, dt);
	if (_bodies[1]) {
		_contactVelocity -= CalculateLocalVelocity(1, dt);
	}

	//Calculates the desired velocity for contact resolution
	CalculateDesiredDeltaVelocity(dt);
}

void Contact::SwapBodies()
{
	//inverts our contact normal
	_contactNormal *= -1;

	//Swaps the bodies using a temp variable
	RigidbodyComponent* temp = _bodies[0];
	_bodies[0] = _bodies[1];
	_bodies[1] = temp;
}

void Contact::MatchAwakeState()
{
	//if we do not have a second rigidbody then return
	if (!_bodies[1]) return;

	//Get the awake states of both bodies
	bool bodyA = _bodies[0]->GetAwake();
	bool bodyB = _bodies[1]->GetAwake();

	//use the ^ (bitwise XOR) operator to determine which one to set awake
	if (bodyA ^ bodyB) {
		if (bodyA) _bodies[1]->SetAwake();
		else _bodies[0]->SetAwake();
	}
}

void Contact::CalculateDesiredDeltaVelocity(float dt)
{
	//A limit to the velocity change
	const static float velocityLimit = 0.25f;

	//Stores the velocity from the acceleration this frame
	float velocityFromAcc = 0.0f;

	//check if our first body is awake and use that as a base value
	if (_bodies[0]->GetAwake()) {
		velocityFromAcc += _bodies[0]->GetLastFrameAcceleration() * dt * _contactNormal;
	}

	//if both bodies are awake subtract the second bodies velocity from the first bodies velocity (now stored in velocityFromAcc)
	if (_bodies[1] && _bodies[1]->GetAwake()) {
		velocityFromAcc -= _bodies[1]->GetLastFrameAcceleration() * dt * _contactNormal;
	}



	float thisRestitution = _restitution;
	//if the contact velocity is very slow then limit our restitution
	if (fabsf(_contactVelocity.x) < velocityLimit) {
		thisRestitution = 0.0f;
	}

	//Calculates our desired velocity
	_desiredDeltaVelocity = -_contactVelocity.x - thisRestitution * (_contactVelocity.x - velocityFromAcc);
}

Vector3 Contact::CalculateLocalVelocity(unsigned bodyIndex, float dt)
{
	//Gets the rigidbody
	RigidbodyComponent* rb = _bodies[bodyIndex];

	//Gets the cross product of the bodies rotation and contact position
	Vector3 velocity = rb->GetRotation() % _relativeContactPosition[bodyIndex];
	//Add the rigidbodies velocity 
	velocity += rb->GetVelocity();

	//Converts our velocity into contact space
	Vector3 contactVelocity = _contactToWorld.TransformTranspose(velocity);

	//Gets the resulting acceleration from last frame
	Vector3 accVelocity = rb->GetLastFrameAcceleration() * dt;

	//Convert the acceleration velocity to contact space
	accVelocity = _contactToWorld.TransformTranspose(accVelocity);

	//ignore the acceleration that is in the contact normal
	accVelocity.x = 0;

	//Add the velocities
	contactVelocity += accVelocity;

	//return our final contact velocity
	return contactVelocity;
}

void Contact::CalculateContactBasis()
{
	Vector3 contactTangent[2];

	//Check if the Z axis is nearer to the x or y axis
	if (fabsf(_contactNormal.x) > fabsf(_contactNormal.y)) {
		//Scaling factor used to normalize our results
		const float s = 1.0f / sqrtf(_contactNormal.x * _contactNormal.x);

		//The new x axis is at right angles to the world Y axis
		contactTangent[0].x = _contactNormal.z * s;
		contactTangent[0].y = 0;
		contactTangent[0].z = -_contactNormal.x * s;

		//The new y axis is at right angles to the new x and z axis
		contactTangent[1].x = _contactNormal.y * contactTangent[0].x;
		contactTangent[1].y = _contactNormal.z * contactTangent[0].x - _contactNormal.x * contactTangent[0].z;
		contactTangent[1].z = -_contactNormal.y * contactTangent[0].x;
	}
	else {
		//Scaling factor used to normalize our results
		const float s = 1.0f / sqrtf(_contactNormal.z * _contactNormal.z + _contactNormal.y * _contactNormal.y);

		//The new x axis is at a right angle to the world X axis
		contactTangent[0].x = 0;
		contactTangent[0].y = -_contactNormal.z * s;
		contactTangent[0].z = _contactNormal.y * s;

		//The new y axis is at a right angle to the new x and z axis
		contactTangent[1].x = _contactNormal.y * contactTangent[0].z - _contactNormal.z * contactTangent[0].y;
		contactTangent[1].y = -_contactNormal.x * contactTangent[0].z;
		contactTangent[1].z = _contactNormal.x * contactTangent[0].y;
	}

	//Creates the matrix to hold these three vectors
	_contactToWorld.SetComponents(_contactNormal, contactTangent[0], contactTangent[1]);
}

void Contact::ApplyVelocityChange(Vector3 velocityChange[2], Vector3 rotationChange[2])
{
	//Store the inertia tensors of our bodies
	Matrix3 inverseInertiaTensor[2];
	_bodies[0]->GetInverseInertiaTensorWorld(&inverseInertiaTensor[0]);
	if (_bodies[1])
		_bodies[1]->GetInverseInertiaTensorWorld(&inverseInertiaTensor[1]);

	//Calculate the impulse to act on each body
	Vector3 impulseContact;

	if (_friction == 0.0f)
	{
		//Calculate a frictionless impulse
		impulseContact = CalculateFrictionlessImpulse(inverseInertiaTensor);
	}
	else
	{
		//Calculate a impulse with friction
		impulseContact = CalculateFrictionImpulse(inverseInertiaTensor);
	}

	// Converts the impulse to world space
	Vector3 impulse = _contactToWorld.Transform(impulseContact);

	// Split the impulse into positional and rotational components
	Vector3 impulsiveTorque = _relativeContactPosition[0] % impulse;
	rotationChange[0] = inverseInertiaTensor[0].Transform(impulsiveTorque);
	velocityChange[0].Zero();
	velocityChange[0].AddScaledVector(impulse, _bodies[0]->GetInverseMass());

	// Apply the changes
	_bodies[0]->AddVelocity(velocityChange[0]);
	_bodies[0]->AddRotation(rotationChange[0]);

	//if we have a second rigidbody then work out the positional and angular changes
	if (_bodies[1])
	{
		
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
	//Limits the angular change
	const float angularLimit = 0.2f;
	float angularMove[2];
	float linearMove[2];

	float totalInertia = 0.0f;
	float linearInertia[2];
	float angularInertia[2];

	//Cycle through both bodies and store the data we need
	for (unsigned i = 0; i < 2; i++) if (_bodies[i])
	{
		//Stores our inertia tensor
		Matrix3 inverseInertiaTensor;
		_bodies[i]->GetInverseInertiaTensorWorld(&inverseInertiaTensor);

		//work out the inertia world matrix with the cross product of our contact position and contact normal
		Vector3 angularInertiaWorld = _relativeContactPosition[i] % _contactNormal;
		//convert it into world space
		angularInertiaWorld = inverseInertiaTensor.Transform(angularInertiaWorld);

		//Cross product it with the contact position
		angularInertiaWorld = angularInertiaWorld % _relativeContactPosition[i];
		//Calculate the final angular inertia
		angularInertia[i] = angularInertiaWorld * _contactNormal;

		//store the mass
		linearInertia[i] = _bodies[i]->GetInverseMass();

		//Increment the total inertia by the updated inertia
		totalInertia += linearInertia[i] + angularInertia[i];
	}

	//cycle through each body and apply the position change
	for (unsigned i = 0; i < 2; i++) if (_bodies[i]) {
		//used to invert the position change on the second body
		float sign = (i == 0) ? 1 : -1;

		//calculate the angular and linear movement based on the penetration and our stored inertia matrices
		angularMove[i] = sign * penetration * (angularInertia[i] / totalInertia);
		linearMove[i] = sign * penetration * (linearInertia[i] / totalInertia);

		Vector3 projection = _relativeContactPosition[i];
		//Adds the contact normal to the project and then scale it by the scalar product of the contact position and contact normal
		projection.AddScaledVector(_contactNormal, -_relativeContactPosition[i].ScalarProduct(_contactNormal));

		//Store the maximum magnitude 
		float maxMagnitude = angularLimit * projection.Magnitude();

		//Clamp our linear movements for extreme angular movements
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

		//Angles dont need to change if the angular move is 0, this is unlikely to happen though unless objects collide exactly
		if (angularMove[i] == 0) {
			angularChange[i].Zero();
		}
		else {
			//Calculate the desired angular direction
			Vector3 targetAngularDirection = _relativeContactPosition[i].VectorProduct(_contactNormal);

			//Stores the bodies inertia tensor
			Matrix3 inverseInertiaTensor;
			_bodies[i]->GetInverseInertiaTensorWorld(&inverseInertiaTensor);

			//Sets our angular change based on the inertia tensor (transformed to world space) multiplied by the angular move and inertia
			angularChange[i] = inverseInertiaTensor.Transform(targetAngularDirection) * (angularMove[i] / angularInertia[i]);
		}

		//Calculate our linear change
		linearChange[i] = _contactNormal * linearMove[i];

		//Store the position and add the contact normal scaled by our movement
		Vector3 pos = _bodies[i]->GetPosition();
		pos.AddScaledVector(_contactNormal, linearMove[i]);
		//Set our new position
		_bodies[i]->SetPosition(pos);

		//Gets the orientation of our body and add the angular change to it and then set the orientation
		Quaternion q = _bodies[i]->GetOrientation();
		q.AddScaledVector(angularChange[i], 1.0f);
		_bodies[i]->SetOrientation(q);

		//calculates the state data for the bodies if they are awake
		if (!_bodies[i]->GetAwake()) _bodies[i]->CalculateDerivedData();
	}
}

Vector3 Contact::CalculateFrictionlessImpulse(Matrix3* inverseIntertiaTensor)
{
	//Gets the cross product of our contact position and contact normal
	Vector3 deltaVelWorld = _relativeContactPosition[0] % _contactNormal;
	//Converts it to world space
	deltaVelWorld = inverseIntertiaTensor[0].Transform(deltaVelWorld);
	deltaVelWorld = deltaVelWorld % _relativeContactPosition[0];

	//Calculates our desired change in velocity in contact coordinates
	float deltaVelocity = deltaVelWorld * _contactNormal;

	//Add the mass to the velocity 
	deltaVelocity += _bodies[0]->GetInverseMass();

	//if we have a second body
	if (_bodies[1]) {
		//perform the same steps as above
		Vector3 deltaVelWorld = _relativeContactPosition[1] % _contactNormal;
		deltaVelWorld = inverseIntertiaTensor[1].Transform(deltaVelWorld);
		deltaVelWorld = deltaVelWorld % _relativeContactPosition[1];

		deltaVelocity += deltaVelWorld * _contactNormal;

		deltaVelocity += _bodies[1]->GetInverseMass();
	}

	//Calculates our impulse 
	Vector3 impulseContact;
	impulseContact.x = _desiredDeltaVelocity / deltaVelocity;
	return impulseContact;
}

Vector3 Contact::CalculateFrictionImpulse(Matrix3* inverseInertiaTensor)
{
	Vector3 impulseContact;
	//Stores our inverse mass
	float inverseMass = _bodies[0]->GetInverseMass();

	//Skews a matrix by the contact position
	Matrix3 impulseToTorque;
	impulseToTorque.SetSkewSymmetric(_relativeContactPosition[0]);

	//Stores the velocity change this frame in world coordinates
	Matrix3 deltaVelWorld = impulseToTorque;
	deltaVelWorld *= inverseInertiaTensor[0];
	deltaVelWorld *= impulseToTorque;
	deltaVelWorld *= -1;

	// Perform the same logic for the second rigidbody
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

	//Calculate the change in velocity
	Matrix3 deltaVelocity = _contactToWorld.Transpose();
	deltaVelocity *= deltaVelWorld;
	deltaVelocity *= _contactToWorld;

	// Add the mass to the velocity linear change
	deltaVelocity._data[0] += inverseMass;
	deltaVelocity._data[4] += inverseMass;
	deltaVelocity._data[8] += inverseMass;

	// Invert the velocity to get our desired impulse
	Matrix3 impulseMatrix = deltaVelocity.Inverse();

	// Find the target velocities to counter the penetration
	Vector3 counterVelocity(_desiredDeltaVelocity,
		-_contactVelocity.y,
		-_contactVelocity.z);

	// Find the impulse to counter target velocities
	impulseContact = impulseMatrix.Transform(counterVelocity);

	// Check if we are exceeding friction
	float planarImpulse = sqrtf(impulseContact.y * impulseContact.y + impulseContact.z * impulseContact.z);
	if (planarImpulse > impulseContact.x * _friction)
	{
		// We need to use dynamic friction
		impulseContact.y /= planarImpulse;
		impulseContact.z /= planarImpulse;

		//calculate the impulse based on our delta velocity and the friction coefficient 
		impulseContact.x = deltaVelocity._data[0] +
			deltaVelocity._data[1] * _friction * impulseContact.y +
			deltaVelocity._data[2] * _friction * impulseContact.z;
		impulseContact.x = _desiredDeltaVelocity / impulseContact.x;

		//Multiply the contact points by friction
		impulseContact.y *= _friction * impulseContact.x;
		impulseContact.z *= _friction * impulseContact.x;
	}
	return impulseContact;
}

