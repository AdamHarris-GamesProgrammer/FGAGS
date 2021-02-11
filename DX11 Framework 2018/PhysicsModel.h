#pragma once
#include "Vector3.h"


class PhysicsModel
{
public:


	virtual void AddForce(const Vector3& force);
	virtual void Update(real dt) = 0;
	virtual void ClearAccumulator();

	real GetMass() const;
	void SetMass(const real mass);
	bool HasFiniteMass() const {
		return _inverseMass >= 0.0f;
	}

	real GetInverseMass() const;
	void SetInverseMass(real val);

	real GetDamping() const;
	void SetDamping(real val);

	Vector3 GetAcceleration() const;
	void SetAcceleration(Vector3 val = Vector3());
	void SetAcceleration(float x, float y, float z);
	Vector3 GetVelocity() const;
	void SetVelocity(Vector3 val = Vector3());
	void SetVelocity(float x, float y, float z);
	Vector3 GetPosition() const;
	void SetPosition(Vector3 val = Vector3());
	void SetPosition(float x, float y, float z);

protected:
	Vector3 _position;
	Vector3 _velocity;
	Vector3 _acceleration;
	
	Vector3 _forceAccumulator;

	real _inverseMass;
	real _linearDamping;




private:



};

