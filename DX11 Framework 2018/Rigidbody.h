#pragma once
#include "Precision.h"
#include "Vector3.h"
#include "Quaternion.h"

#include "Matrix3.h"
#include "Matrix4.h"

class Rigidbody
{
public:
	void CalculateDerivedData();


	void SetInertiaTensor(const Matrix3& inertiaTensor) { _inverseInertiaTensor.SetInverse(inertiaTensor); }
	void SetMass(const real mass) 
	{
		assert(mass != 0);
		_inverseMass = ((real)1.0) / mass;
	}
	real GetMass() const {
		if (_inverseMass == 0) {
			return REAL_MAX;
		}
		else {
			return ((real)1.0) / _inverseMass;
		}
	}

	void SetInverseMass(const real inverseMass) { _inverseMass = inverseMass; }
	real GetInverseMass() { return _inverseMass; }

	bool HasFiniteMass() const {
		return _inverseMass > 0.0f;
	}

	void GetInertiaTensor(Matrix3* inertiaTensor) const {
		inertiaTensor->SetInverse(_inverseInertiaTensorWorld);
	}

	Matrix3 GetInertiaTensor() const {
		Matrix3 it;
		GetInertiaTensor(&it);
		return it;
	}

	void SetInverseInertiaTensor(const Matrix3& inverseIntertiaTensor) {
		_inverseInertiaTensor = inverseIntertiaTensor;
	}

	void GetInverseInertiaTensor(Matrix3* inverseInertiaTensor) const {
		*inverseInertiaTensor = _inverseInertiaTensor;
	}

	Matrix3 GetInverseInertiaTensor() const {
		return _inverseInertiaTensor;
	}

	void GetInverseInertiaTensorWorld(Matrix3* inverseInertiaTensor) const {
		*inverseInertiaTensor = _inverseInertiaTensorWorld;
	}
	Matrix3 GetInverseInertiaTensorWorld() const {
		return _inverseInertiaTensorWorld;
	}

	void SetDamping(const real linearDamping, const real angularDamping) {
		_angularDamping = angularDamping;
		_linearDamping = linearDamping;
	}

	void SetLinearDamping(const real linearDamping) {
		_linearDamping = linearDamping;
	}

	void SetAngularDamping(const real angularDamping) {
		_angularDamping = angularDamping;
	}

	real GetAngularDamping() {
		return _angularDamping;
	}

	real GetLinearDamping() {
		return _linearDamping;
	}

	void SetPosition(const Vector3& position) {
		_position = position;
	}

	void SetPosition(const real x, const real y, const real z) {
		_position.x = x;
		_position.y = y;
		_position.z = z;
	}
	
	Vector3 GetPosition() const {
		return _position;
	}

	void SetOrientation(const Quaternion& orientation) {
		_orientation = orientation;
		_orientation.Normalize();
	}

	void SetOrientation(const real r, const real i, const real j, const real k) {
		_orientation._r = r;
		_orientation._i = i;
		_orientation._j = j;
		_orientation._k = k;
	}

	Quaternion GetOrientation() const {
		return _orientation;
	}
	 
	void GetOrientation(Quaternion* orientation) const {
		*orientation = _orientation;
	}

	void GetOrientation(Matrix3* matrix) const {
		GetOrientation(matrix->_data);
	}

	void GetOrientation(real matrix[9]) const {
		matrix[0] = _transformMatrix._data[0];
		matrix[1] = _transformMatrix._data[1];
		matrix[2] = _transformMatrix._data[2];

		matrix[3] = _transformMatrix._data[4];
		matrix[4] = _transformMatrix._data[5];
		matrix[5] = _transformMatrix._data[6];

		matrix[6] = _transformMatrix._data[8];
		matrix[7] = _transformMatrix._data[9];
		matrix[8] = _transformMatrix._data[10];
	}
 
	void GetTransform(Matrix4* transform) const {
		memcpy(transform, &_transformMatrix._data, sizeof(real) * 12);
	}

	void GetTransform(real matrix[16]) const {
		memcpy(matrix, _transformMatrix._data, sizeof(real) * 12);
		matrix[12] = matrix[13] = matrix[14] = 0;
		matrix[15] = 1;
	}

	Matrix4 GetTransform() const {
		return _transformMatrix;
	}

	Vector3 GetPointInWorldSpace(const Vector3& point) const {
		return _transformMatrix.Transform(point);
	}

	Vector3 GetDirectionInWorldSpace(const Vector3& point) const {
		return _transformMatrix.TransformDirection(point);
	}

	Vector3 GetDirectionInLocalSpace(const Vector3& point) const {
		return _transformMatrix.TransformInverDirection(point);
	}

	void SetVelocity(const Vector3& velocity) {
		_velocity = velocity;
	}

	void SetVelocity(const real x, const real y, const real z) {
		_velocity.x = x;
		_velocity.y = y;
		_velocity.z = z;
	}

	Vector3 GetVelocity() const {
		return _velocity;
	}

	void AddVelocity(const Vector3& deltaVelocity) {
		_velocity += deltaVelocity;
	}

	void SetRotation(const Vector3& rotation) {
		_rotation = rotation;
	}

	void SetRotation(const real x, const real y, const real z) {
		_rotation.x = x;
		_rotation.y = y;
		_rotation.z = z;
	}

	Vector3 GetRotation() const {
		return _rotation;
	}

	void AddRotation(const Vector3& deltaRotation) {
		_rotation += deltaRotation;
	}

	void SetAwake(const bool awake = true) {
		if (awake) {
			_isAwake = true;
			_motion = _sleepEpsilon * 2.0f;
			
		}
		else
		{
			_isAwake = false;
			_velocity.Zero();
			_rotation.Zero();
		}
	}

	void SetCanSleep(const bool canSleep) {
		_canSleep = canSleep;

		if (!canSleep && !_isAwake) SetAwake();
	}

	Vector3 GetLastFrameAcceleration() const {
		return _previousAcceleration;
	}

	void AddTorque(const Vector3& torque) {
		_torqueAccumulator += torque;
		_isAwake = true;
	}

	void SetAcceleration(const Vector3& acceleration) {
		_acceleration = acceleration;
	}

	void SetAcceleration(const real x, const real y, const real z) {
		_acceleration.x = x;
		_acceleration.y = y;
		_acceleration.z = z;
	}

	Vector3 GetAcceleration() {
		return _acceleration;
	}

	//Add Forces
	void AddForce(const Vector3& force);
	void AddForceAtPoint(const Vector3& force, const Vector3& point);
	void AddForceAtBodyPoint(const Vector3& force, const Vector3& point);

	void ClearAccumulators();

	void Integrate(real dt);

protected:
	Vector3 _forceAccumulator;
	Vector3 _torqueAccumulator;

	bool _isAwake;


	real _inverseMass;

	real _linearDamping;
	real _angularDamping;

	Vector3 _position;

	Quaternion _orientation;

	Vector3 _velocity;

	Vector3 _rotation;

	Matrix4 _transformMatrix;

	Vector3 _acceleration;

	Vector3 _previousAcceleration;

	real _motion;

	bool _canSleep;

	Matrix3 _inverseInertiaTensor;
	Matrix3 _inverseInertiaTensorWorld;



	//TODO: Abstract sleeping stuff out
	real _sleepEpsilon = 0.1;

	void SetSleepEpsilon(real value) { _sleepEpsilon = value; }

	real GetSleepEpsilon() { return _sleepEpsilon; }
private:


};

