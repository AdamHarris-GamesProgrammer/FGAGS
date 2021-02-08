#include "Rigidbody.h"


static inline void CalculateTransformMatrix(
	Matrix4& transformMatrix, const Vector3& position, const Quaternion& orientation)
{
	transformMatrix._data[0] = 1 - 2 * orientation._j * orientation._j - 2 * orientation._k * orientation._k;
	transformMatrix._data[1] = 2 * orientation._i * orientation._j - 2 * orientation._r * orientation._k;
	transformMatrix._data[2] = 2 * orientation._i * orientation._k + 2 * orientation._r * orientation._j;
	transformMatrix._data[3] = position.x;
	transformMatrix._data[4] = 2 * orientation._i * orientation._j + 2 * orientation._r * orientation._k;
	transformMatrix._data[5] = 1 - 2 * orientation._i * orientation._i - 2 * orientation._k * orientation._k;
	transformMatrix._data[6] = 2 * orientation._j * orientation._k - 2 * orientation._r * orientation._i;
	transformMatrix._data[7] = position.y;
	transformMatrix._data[8] = 2 * orientation._i * orientation._k - 2 * orientation._r * orientation._j;
	transformMatrix._data[9] = 2 * orientation._j * orientation._k + 2 * orientation._r * orientation._i;
	transformMatrix._data[10] = 1 - 2 * orientation._i * orientation._i - 2 * orientation._j * orientation._j;
	transformMatrix._data[11] = position.z;
	transformMatrix._data[11] = position.z;

}

static inline void CalculateInertiaTensor(Matrix3& iitWorld, const Quaternion& q, const Matrix3& iitBody, const Matrix4& rotmat) {
	real t4 = rotmat._data[0] * iitBody._data[0] + rotmat._data[1] * iitBody._data[3] + rotmat._data[2] * iitBody._data[6];
	real t9 = rotmat._data[0] * iitBody._data[1] + rotmat._data[1] * iitBody._data[4] + rotmat._data[2] * iitBody._data[7];
	real t14 = rotmat._data[0] * iitBody._data[2] + rotmat._data[1] * iitBody._data[5] + rotmat._data[2] * iitBody._data[8];
	real t28 = rotmat._data[4] * iitBody._data[0] + rotmat._data[5] * iitBody._data[3] + rotmat._data[6] * iitBody._data[6];
	real t33 = rotmat._data[4] * iitBody._data[1] + rotmat._data[5] * iitBody._data[4] + rotmat._data[6] * iitBody._data[7];
	real t38 = rotmat._data[4] * iitBody._data[2] + rotmat._data[5] * iitBody._data[5] + rotmat._data[6] * iitBody._data[8];
	real t52 = rotmat._data[8] * iitBody._data[0] + rotmat._data[9] * iitBody._data[3] + rotmat._data[10] * iitBody._data[6];
	real t57 = rotmat._data[8] * iitBody._data[1] + rotmat._data[9] * iitBody._data[4] + rotmat._data[10] * iitBody._data[7];
	real t62 = rotmat._data[8] * iitBody._data[2] + rotmat._data[9] * iitBody._data[5] + rotmat._data[10] * iitBody._data[8];


	iitWorld._data[0] = t4 * rotmat._data[0] + t9 * rotmat._data[1] + t14 * rotmat._data[2];
	iitWorld._data[1] = t4 * rotmat._data[4] + t9 * rotmat._data[5] + t14 * rotmat._data[6];
	iitWorld._data[2] = t4 * rotmat._data[8] + t9 * rotmat._data[9] + t14 * rotmat._data[10];
	iitWorld._data[3] = t28 * rotmat._data[0] + t33 * rotmat._data[1] + t38 * rotmat._data[2];
	iitWorld._data[4] = t28 * rotmat._data[4] + t33 * rotmat._data[5] + t38 * rotmat._data[6];
	iitWorld._data[5] = t28 * rotmat._data[8] + t33 * rotmat._data[9] + t38 * rotmat._data[10];
	iitWorld._data[6] = t52 * rotmat._data[0] + t57 * rotmat._data[1] + t62 * rotmat._data[2];
	iitWorld._data[7] = t52 * rotmat._data[4] + t57 * rotmat._data[5] + t62 * rotmat._data[6];
	iitWorld._data[8] = t52 * rotmat._data[8] + t57 * rotmat._data[9] + t62 * rotmat._data[10];
}

void Rigidbody::CalculateDerivedData()
{
	_orientation.Normalize();

	CalculateTransformMatrix(_transformMatrix, _position, _orientation);

	CalculateInertiaTensor(_inverseInertiaTensorWorld, _orientation, _inverseInertiaTensor, _transformMatrix);
}

void Rigidbody::AddForce(const Vector3& force)
{
	_forceAccumulator += force;
	_isAwake = true;
}

void Rigidbody::AddForceAtPoint(const Vector3& force, const Vector3& point)
{
	Vector3 pt = point;

	_forceAccumulator += force;
	_forceAccumulator += pt % force;

	_isAwake = true;
}

void Rigidbody::AddForceAtBodyPoint(const Vector3& force, const Vector3& point)
{
	Vector3 pt = GetPointInWorldSpace(point);
	AddForceAtPoint(force, pt);

	_isAwake = true;
}

void Rigidbody::ClearAccumulators()
{
	_forceAccumulator.Zero();
	_torqueAccumulator.Zero();
}

void Rigidbody::Integrate(real dt)
{
	//Calculate linear acceleration from the force inputs
	_previousAcceleration = _acceleration;
	_previousAcceleration.AddScaledVector(_forceAccumulator, _inverseMass);

	//Calculate angular acceleration from torque forces
	Vector3 angularAcceleration = _inverseInertiaTensorWorld.Transform(_torqueAccumulator);

	//Calculate Velocity and rotation changes
	_velocity.AddScaledVector(_previousAcceleration, dt);
	_rotation.AddScaledVector(angularAcceleration, dt);

	//Applies damping to velocity a
	_velocity *= real_pow(_linearDamping, dt);
	_rotation *= real_pow(_angularDamping, dt);

	//Updates position
	_position.AddScaledVector(_velocity, dt);

	//Updates Orientation
	_orientation.AddScaledVector(_rotation, dt);

	//Normalize the orientation and update the transform matrix
	CalculateDerivedData();

	//Clear the force accumulators
	ClearAccumulators();

	if (_canSleep) {
		real currentMotion = _velocity.ScalarProduct(_velocity) + _rotation.ScalarProduct(_rotation);
		real bias = real_pow(0.5, dt);

		_motion = bias * _motion + (1 - bias) * currentMotion;

		if (_motion < _sleepEpsilon) SetAwake(false);
		else if (_motion > 10 * _sleepEpsilon) _motion = 10 * _sleepEpsilon;
	}
}


