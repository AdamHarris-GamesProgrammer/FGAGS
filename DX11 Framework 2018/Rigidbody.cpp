#include "Rigidbody.h"
#include "Object.h"

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

}

static inline void CalculateInertiaTensor(Matrix3& iitWorld, const Quaternion& q, const Matrix3& iitBody, const Matrix4& rotmat) {
	float t4 = rotmat._data[0] * iitBody._data[0] + rotmat._data[1] * iitBody._data[3] + rotmat._data[2] * iitBody._data[6];
	float t9 = rotmat._data[0] * iitBody._data[1] + rotmat._data[1] * iitBody._data[4] + rotmat._data[2] * iitBody._data[7];
	float t14 = rotmat._data[0] * iitBody._data[2] + rotmat._data[1] * iitBody._data[5] + rotmat._data[2] * iitBody._data[8];
	float t28 = rotmat._data[4] * iitBody._data[0] + rotmat._data[5] * iitBody._data[3] + rotmat._data[6] * iitBody._data[6];
	float t33 = rotmat._data[4] * iitBody._data[1] + rotmat._data[5] * iitBody._data[4] + rotmat._data[6] * iitBody._data[7];
	float t38 = rotmat._data[4] * iitBody._data[2] + rotmat._data[5] * iitBody._data[5] + rotmat._data[6] * iitBody._data[8];
	float t52 = rotmat._data[8] * iitBody._data[0] + rotmat._data[9] * iitBody._data[3] + rotmat._data[10] * iitBody._data[6];
	float t57 = rotmat._data[8] * iitBody._data[1] + rotmat._data[9] * iitBody._data[4] + rotmat._data[10] * iitBody._data[7];
	float t62 = rotmat._data[8] * iitBody._data[2] + rotmat._data[9] * iitBody._data[5] + rotmat._data[10] * iitBody._data[8];


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

void RigidbodyComponent::CalculateDerivedData()
{
	_orientation.Normalize();

	CalculateTransformMatrix(_transformMatrix, _position, _orientation);
	CalculateInertiaTensor(_inverseInertiaTensorWorld, _orientation, _inverseInertiaTensor, _transformMatrix);
}

void RigidbodyComponent::SetCubeInertiaTensor()
{
	TransformComponent* pTransformComponent = dynamic_cast<TransformComponent*>(_pOwner->GetComponent(Transform));

	Vector3 scale = pTransformComponent->GetScale();

	//Calculate the extents from the center (half the scale)
	scale.x /= 2.0f;
	scale.y /= 2.0f;
	scale.z /= 2.0f;

	Matrix3 cubeTensor;
	cubeTensor._data[0] = 1 / 12 * GetMass() * (scale.y * scale.y + scale.z * scale.z);
	cubeTensor._data[4] = 1 / 12 * GetMass() * (scale.x * scale.x + scale.z * scale.z);
	cubeTensor._data[8] = 1 / 12 * GetMass() * (scale.x * scale.x + scale.y * scale.y);
	
	SetInertiaTensor(cubeTensor);
}

void RigidbodyComponent::SetSphereInertiaTensor()
{
	TransformComponent* pTransformComponent = dynamic_cast<TransformComponent*>(_pOwner->GetComponent(Transform));

	float radius = pTransformComponent->GetScale().x;

	Matrix3 sphereTensor;
	sphereTensor._data[0] = 2 / 5 * GetMass() * (radius * radius);
	sphereTensor._data[4] = 2 / 5 * GetMass() * (radius * radius);
	sphereTensor._data[8] = 2 / 5 * GetMass() * (radius * radius);

	SetInertiaTensor(sphereTensor);
}

void RigidbodyComponent::AddForce(const Vector3& force)
{
	PhysicsModelComponent::AddForce(force);
	_isAwake = true;
}

void RigidbodyComponent::AddForceAtPoint(const Vector3& force, const Vector3& point)
{
	Vector3 pt = point;

	_forceAccumulator += force;
	_forceAccumulator += pt % force;

	_isAwake = true;
}

void RigidbodyComponent::AddForceAtBodyPoint(const Vector3& force, const Vector3& point)
{
	Vector3 pt = GetPointInWorldSpace(point);
	AddForceAtPoint(force, pt);

	_isAwake = true;
}

void RigidbodyComponent::ClearAccumulator()
{
	PhysicsModelComponent::ClearAccumulator();
	_torqueAccumulator.Zero();
}

void RigidbodyComponent::Update(float dt)
{
	if (!_isAwake) return;

	//Calculate linear acceleration from the force inputs
	_previousAcceleration = _acceleration;
	_previousAcceleration.AddScaledVector(_forceAccumulator, _inverseMass);

	//Calculate angular acceleration from torque forces
	Vector3 angularAcceleration = _inverseInertiaTensorWorld.Transform(_torqueAccumulator);

	//Calculate Velocity and rotation changes
	_velocity.AddScaledVector(_previousAcceleration, dt);
	_rotation.AddScaledVector(angularAcceleration, dt);

	//Applies damping to velocity a
	_velocity *= powf(_linearDamping, dt);
	_rotation *= powf(_angularDamping, dt);

	//Updates position
	_position.AddScaledVector(_velocity, dt);

	//Updates Orientation
	_orientation.AddScaledVector(_rotation, dt);

	//Normalize the orientation and update the transform matrix
	CalculateDerivedData();

	//Clear the force accumulators
	ClearAccumulator();

	_pTransformComponent->SetPosition(_position);
	_pTransformComponent->SetOrientation(_orientation);

	CheckSleep(_velocity.ScalarProduct(_velocity) + _rotation.ScalarProduct(_rotation), dt);

}

void RigidbodyComponent::Initialize()
{
	PhysicsModelComponent::Initialize();

	SetPosition(_pTransformComponent->GetPosition());
	SetRotation(_pTransformComponent->GetRotation());
	SetCanSleep(true);
	SetAwake(false);
	SetAngularDamping(0.8f);
	SetLinearDamping(0.95f);

	Matrix3 tensor;

	float coeff = 0.4 * GetMass() * 1.0 * 1.0;
	tensor.SetInertiaTensorCoeffs(coeff, coeff, coeff);
	tensor.SetBlockInertiaTensor(Vector3(1.0, 1.0, 1.0), 5.0);
	SetInertiaTensor(tensor);

	ClearAccumulator();
	CalculateDerivedData();
}

