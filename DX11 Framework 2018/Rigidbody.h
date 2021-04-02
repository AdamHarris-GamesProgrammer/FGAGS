#pragma once
#include "Vector3.h"
#include "Quaternion.h"

#include "PhysicsModel.h"
#include "Transform.h"

#include "Matrix3.h"
#include "Matrix4.h"

class Object;

class RigidbodyComponent : public PhysicsModelComponent
{
public:
	RigidbodyComponent(Object* owner) : PhysicsModelComponent(owner, Rigidbody, 50.0f)
	{
		Initialize();
	}

	void CalculateDerivedData();


	void SetInertiaTensor(const Matrix3& inertiaTensor) { _inverseInertiaTensor.SetInverse(inertiaTensor); }

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

	void SetDamping(const float linearDamping, const float angularDamping) {
		_angularDamping = angularDamping;
		_linearDamping = linearDamping;
	}

	void SetLinearDamping(const float linearDamping) {
		_linearDamping = linearDamping;
	}

	void SetAngularDamping(const float angularDamping) {
		_angularDamping = angularDamping;
	}

	float GetAngularDamping() {
		return _angularDamping;
	}



	void SetOrientation(const Quaternion& orientation) {
		_orientation = orientation;
		_orientation.Normalize();
	}

	void SetOrientation(const float r, const float i, const float j, const float k) {
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

	void GetOrientation(float matrix[9]) const {
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
		memcpy(transform, &_transformMatrix._data, sizeof(float) * 12);
	}

	void GetTransform(float matrix[16]) const {
		memcpy(matrix, _transformMatrix._data, sizeof(float) * 12);
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

	void AddVelocity(const Vector3& deltaVelocity) {
		_velocity += deltaVelocity;
	}

	void SetRotation(const Vector3& rotation) {
		_rotation = rotation;
		_orientation._i = rotation.x;
		_orientation._j = rotation.y;
		_orientation._k = rotation.z;
	}

	void SetRotation(const float x, const float y, const float z) {
		_rotation.x = x;
		_rotation.y = y;
		_rotation.z = z;
		_orientation._i = x;
		_orientation._j = y;
		_orientation._k = z;
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


	bool GetAwake() const {
		return _isAwake;
	}

	//Add Forces
	void AddForce(const Vector3& force) override;
	void AddForceAtPoint(const Vector3& force, const Vector3& point);
	void AddForceAtBodyPoint(const Vector3& force, const Vector3& point);

	void ClearAccumulator() override;

	void Update(float dt);

private:
	void Initialize() override;

protected:
	Vector3 _torqueAccumulator;

	bool _isAwake;


	float _angularDamping;


	Quaternion _orientation;


	Vector3 _rotation;

	Matrix4 _transformMatrix;

	Vector3 _previousAcceleration;

	float _motion;

	bool _canSleep;

	Matrix3 _inverseInertiaTensor;
	Matrix3 _inverseInertiaTensorWorld;

	//TODO: Abstract sleeping stuff out physics model class
	float _sleepEpsilon = 0.1;

	void SetSleepEpsilon(float value) { _sleepEpsilon = value; }

	float GetSleepEpsilon() { return _sleepEpsilon; }
private:


};

