#pragma once

#include "Vector3.h"
#include "Precision.h"
#include <assert.h>


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



public:
	void Integrate(real dt);

	void ClearAccumulator();

	void AddForce(const Vector3& force);



	real Mass() const;
	void Mass(const real mass);
	bool HasFiniteMass() const {
		return _inverseMass >= 0.0f;
	}
	real InverseMass() const { return _inverseMass; }
	void InverseMass(real val) { _inverseMass = val; }
	real Damping() const { return _damping; }
	void Damping(real val) { _damping = val; }
	Vector3 Acceleration() const { return _acceleration; }
	void Acceleration(Vector3 val) { _acceleration = val; }
	Vector3 Velocity() const { return _velocity; }
	void Velocity(Vector3 val) { _velocity = val; }
	Vector3 Position() const { return _position; }
	void Position(Vector3 val) { _position = val; }
};

