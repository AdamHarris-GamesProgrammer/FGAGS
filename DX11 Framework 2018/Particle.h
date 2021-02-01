#pragma once

#include "Vector3.h"
#include "Precision.h"
#include <assert.h>
#include "Transform.h"


class Particle
{
protected:
	//Position in world space
	Vector3 _position;

	//Linear velocity in world space
	Vector3 _velocity;

	//Holds the acceleration of the particle.
	Vector3 _acceleration;

	Vector3 _forceAccumulator;

	//Holds the amount of damping that is applied to linear motion. Acts as a drag force
	real _damping;

	//Holds the inverse mass of the particle it is more useful to hold the inverse mass because integration is simpler
	real _inverseMass;

	Transform* _transform;

public:
	Particle(Transform* transform) : _transform(transform) {}

	void Update(real dt);

	void ClearAccumulator();

	void AddForce(const Vector3& force);



	real GetMass() const;
	void SetMass(const real mass);
	bool HasFiniteMass() const {
		return _inverseMass >= 0.0f;
	}
	
	real GetInverseMass() const { return _inverseMass; }
	void SetInverseMass(real val) { _inverseMass = val; }
	
	real GetDamping() const { return _damping; }
	void SetDamping(real val) { _damping = val; }
	
	Vector3 GetAcceleration() const { return _acceleration; }
	void SetAcceleration(Vector3 val = Vector3()) { _acceleration = val; }
	void SetAcceleration(float x, float y, float z) { _acceleration.x = x; _acceleration.y = y; _acceleration.z = z; }
	Vector3 GetVelocity() const { return _velocity; }
	void SetVelocity(Vector3 val = Vector3()) { _velocity = val; }
	void SetVelocity(float x, float y, float z) { _velocity.x = x; _velocity.y = y; _velocity.z = z; }
	Vector3 GetPosition() const { return _position; }
	void SetPosition(Vector3 val = Vector3()) { _transform->SetPosition(val); }
	void SetPosition(float x, float y, float z) { _transform->SetPosition(x, y, z); }
};

