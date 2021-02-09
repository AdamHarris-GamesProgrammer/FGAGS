#pragma once
#include "Vector3.h"
#include "Precision.h"

class Contact {
	friend class ContactResolver;
public:
	Rigidbody* _bodies[2];

	real _friction;
	real _restitution;


	Vector3 _contactPoint;
	Vector3 _contactNormal;
	real _penetration;

	void SetBodyData(Rigidbody* a, Rigidbody* b, real friction, real restitution) {
		_bodies[0] = a;
		_bodies[1] = b;
		_friction = friction;
		_restitution = restitution;
	}

protected:
	Matrix3 _contactToWorld;
	Vector3 _contactVelocity;
	real _desiredDeltaVelocity;
	Vector3 _relativeContactPosition[2];

	void CalculateInternals(real dt) {
		if (_bodies[0]) SwapBodies();
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

	void SwapBodies() {
		_contactNormal *= -1;

		Rigidbody* temp = _bodies[0];
		_bodies[0] = _bodies[1];
		_bodies[1] = temp;
	}

	void MatchAwakeState() {
		if (!_bodies[1]) return;

		bool bodyA = _bodies[0]->GetAwake();
		bool bodyB = _bodies[1]->GetAwake();

		if (bodyA ^ bodyB) {
			if (bodyA) _bodies[1]->SetAwake();
			else _bodies[0]->SetAwake();
		}
	}

	void CalculateDesiredDeltaVelocity(real dt) {
		const static real velocityLimit = (real)0.25f;

		real velocityFromAcc = 0;

		if (_bodies[0]->GetAwake()) {
			velocityFromAcc += _bodies[0]->GetLastFrameAcceleration() * dt * _contactNormal;
		}

		if (_bodies[1] && _bodies[1]->GetAwake()) {
			velocityFromAcc -= _bodies[1]->GetLastFrameAcceleration() * dt * _contactNormal;
		}

		real thisRestitution = _restitution;
		if (real_abs(_contactVelocity.x) < velocityLimit) {
			thisRestitution = (real)0.0f;
		}

		_desiredDeltaVelocity = -_contactVelocity.x - thisRestitution * (_contactVelocity.x - velocityFromAcc);
	}

	Vector3 CalculateLocalVelocity(unsigned bodyIndex, real dt) {
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

	void CalculateContactBasis() {
		Vector3 contactTangent[2];

		if (real_abs(_contactNormal.x) > real_abs(_contactNormal.y)) {
			//Scaling factor
			const real s = (real)1.0f / real_sqrt(_contactNormal.x * _contactNormal.x);

			//The new x-axis is at right angles to the world Y-axis
			contactTangent[0].x = _contactNormal.z * s;
			contactTangent[0].y = 0;
			contactTangent[0].z = -_contactNormal.x * s;

			contactTangent[1].x = _contactNormal.y * contactTangent[0].x;
			contactTangent[1].y = _contactNormal.z * contactTangent[0].x - _contactNormal.x * contactTangent[0].z;
			contactTangent[1].z = -_contactNormal.y * contactTangent[0].x;
		}
		else {
			const real s = (real)1.0 / real_sqrt(_contactNormal.z * _contactNormal.z + _contactNormal.y * _contactNormal.y);

			contactTangent[0].x = 0;
			contactTangent[0].y = -_contactNormal.z * s;
			contactTangent[0].z = _contactNormal.y * s;

			contactTangent[1].x = _contactNormal.y * contactTangent[0].z - _contactNormal.z * contactTangent[0].y;
			contactTangent[1].y = -_contactNormal.x * contactTangent[0].z;
			contactTangent[1].z = _contactNormal.x * contactTangent[0].y;
		}

		_contactToWorld.SetComponents(_contactNormal, contactTangent[0], contactTangent[1]);
	}

	void ApplyVelocityChange(Vector3 velocityChange[2], Vector3 rotationChange[2]) {
		// Get hold of the inverse mass and inverse inertia tensor, both in
// world coordinates.
		Matrix3 inverseInertiaTensor[2];
		_bodies[0]->GetInverseInertiaTensorWorld(&inverseInertiaTensor[0]);
		if (_bodies[1])
			_bodies[1]->GetInverseInertiaTensorWorld(&inverseInertiaTensor[1]);

		// We will calculate the impulse for each contact axis
		Vector3 impulseContact;

		if (_friction == (real)0.0)
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

	void ApplyPositionChange(Vector3 linearChange[2], Vector3 angularChange[2], real penetration) {
		const real angularLimit = (real)0.2f;
		real angularMove[2];
		real linearMove[2];

		real totalInertia = 0;
		real linearInertia[2];
		real angularInertia[2];

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
	}

	Vector3 CalculateFrictionlessImpulse(Matrix3* inverseIntertiaTensor) {
		Vector3 impulseContact;

		Vector3 deltaVelWorld = _relativeContactPosition[0] % _contactNormal;
		deltaVelWorld = inverseIntertiaTensor[0].Transform(deltaVelWorld);
		deltaVelWorld = deltaVelWorld % _relativeContactPosition[0];

		real deltaVelocity = deltaVelWorld * _contactNormal;

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

	Vector3 CalculateFrictionImpulse(Matrix3* inverseInertiaTensor) {
		Vector3 impulseContact;
		real inverseMass = _bodies[0]->GetInverseMass();

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
		real planarImpulse = real_sqrt(
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

private:


	class ContactResolver {
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
		ContactResolver(unsigned iterations, real velocityEps = (real)0.01, real positionEps = (real)0.01) {
			SetIterations(iterations);
			SetEpsilon(velocityEps, positionEps);
		}
		ContactResolver(unsigned velocityIt, unsigned positionIt, real positionEp = (real)0.01, real velocityEp = (real)0.01) {
			SetIterations(positionIterations, velocityIterations);
			SetEpsilon(velocityEp, positionEp);
		}

		bool IsValid() {
			return(velocityIterations > 0) && (positionIterations > 0) && (positionEpsilon >= 0.0f) && (velocityEpsilon >= 0.0f);
		}

		void SetIterations(unsigned velocityIt, unsigned positionIt) {
			positionIterations = positionIt;
			velocityIterations = velocityIt;
		}

		void SetIterations(unsigned iterations) {
			positionIterations = iterations;
			velocityIterations = iterations;
		}

		void SetEpsilon(real velocityEp, real positionEp) {
			velocityEpsilon = velocityEp;
			positionEpsilon = positionEp;
		}

		void ResolveContacts(Contact* contacts, unsigned numContacts, real dt) {
			if (numContacts == 0) return;

			PrepareContacts(contacts, numContacts, dt);

			AdjustPositions(contacts, numContacts, dt);

			AdjustVelocities(contacts, numContacts, dt);
		}


	protected:

		void AdjustVelocities(Contact* contacts, unsigned numContacts, real dt)
		{
			Vector3 velocityChange[2], rotationChange[2];
			Vector3 deltaVel;

			velocityIterationsUsed = 0;

			while (velocityIterationsUsed < velocityIterations)
			{
				real max = velocityEpsilon;
				unsigned index = numContacts;
				for (unsigned i = 0; i < numContacts; i++) {
					if (contacts[i]._desiredDeltaVelocity > max) {
						max = contacts[i]._desiredDeltaVelocity;
						index = i;
					}
				}
				if (index == numContacts) break;

				contacts[index].MatchAwakeState();
				contacts[index].ApplyVelocityChange(velocityChange, rotationChange);

				for (unsigned i = 0; i < numContacts; i++) {
					for (unsigned j = 0; j < numContacts; j++) if (contacts[i]._bodies[j]) {
						for (unsigned k = 0; k < numContacts; k++) {
							if (contacts[i]._bodies[j] == contacts[index]._bodies[k]) {
								deltaVel = velocityChange[k] + rotationChange[k].VectorProduct(contacts[i]._relativeContactPosition[j]);

								contacts[i]._contactVelocity += contacts[i]._contactToWorld.TransformTranspose(deltaVel) * (j ? -1 : 1);
								contacts[i].CalculateDesiredDeltaVelocity(dt);
							}
						}
					}
				}
				velocityIterationsUsed++;
			}
			
		}



		void AdjustPositions(Contact* contacts, unsigned numContacts, real dt)
		{
			unsigned i, index;
			Vector3 linearChange[2], angularChange[2];
			real max;
			Vector3 deltaPosition;


			positionIterationsUsed = 0;
			while (positionIterationsUsed < positionIterations) {

				//Find the biggest penetration
				max = positionEpsilon;
				index = numContacts;
				for (i = 0; i < numContacts; i++) {
					if (contacts[i]._penetration > max) {
						max = contacts[i]._penetration;
						index = i;
					}
				}

				if (index == numContacts) break;

				contacts[index].MatchAwakeState();

				contacts[index].ApplyPositionChange(linearChange, angularChange, max);

				for (i = 0; i < numContacts; i++) {
					for (unsigned j = 0; j < 2; j++) if(contacts[i]._bodies[j]) {
						for (unsigned k = 0; k < 2; k++)
						{
							if (contacts[i]._bodies[j] == contacts[index]._bodies[k]) {
								deltaPosition = linearChange[k] + angularChange[k].VectorProduct(contacts[i]._relativeContactPosition[j]);

								contacts[i]._penetration += deltaPosition.ScalarProduct(contacts[i]._contactNormal) * (j ? 1 : -1);
							}
						}
					}
				}
				positionIterationsUsed++;
			}
		}



		void PrepareContacts(Contact* contacts, unsigned numContacts, real dt)
		{
			Contact* lastContact = contacts + numContacts;

			for (Contact* contact = contacts; contact < lastContact; contact++) {
				contact->CalculateInternals(dt);
			}
		}


	};
};