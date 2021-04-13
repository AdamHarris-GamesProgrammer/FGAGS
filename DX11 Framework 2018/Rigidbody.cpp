#include "Rigidbody.h"
#include "Object.h"

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
	//normalizes our orientation, needed for calculations
	_orientation.Normalize();

	_transformMatrix.SetOrientationAndPosition(_orientation, _position);
	CalculateInertiaTensor(_inverseInertiaTensorWorld, _orientation, _inverseInertiaTensor, _transformMatrix);
}

void RigidbodyComponent::EndUpdate(float currMot, float dt)
{
	//Normalize the orientation and update the transform matrix
	CalculateDerivedData();

	PhysicsModelComponent::EndUpdate(currMot, dt);
	//Sets the orientation for our transform component
	_pTransformComponent->SetOrientation(_orientation);
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
	cubeTensor.SetBlockInertiaTensor(scale, GetMass());
	
	SetInertiaTensor(cubeTensor);
}

void RigidbodyComponent::SetSphereInertiaTensor()
{
	//Gets the scale of our object
	float radius = _pTransformComponent->GetScale().x;

	//Calculates the tensor for a sphere
	Matrix3 sphereTensor;
	sphereTensor._data[0] = 2 / 5 * GetMass() * (radius * radius);
	sphereTensor._data[4] = 2 / 5 * GetMass() * (radius * radius);
	sphereTensor._data[8] = 2 / 5 * GetMass() * (radius * radius);

	SetInertiaTensor(sphereTensor);
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
	//Adds a force to a specific point on the Rigidbody
	Vector3 pt = GetPointInWorldSpace(point);
	AddForceAtPoint(force, pt);

	_isAwake = true;
}

void RigidbodyComponent::ClearAccumulator()
{
	//clears our force accumulator and our torque accumulator
	PhysicsModelComponent::ClearAccumulator();
	_torqueAccumulator.Zero();
}

void RigidbodyComponent::Update(float dt)
{
	if (!BeginUpdate(dt)) return;

	//Calculate linear acceleration from the force inputs
	_previousAcceleration = _acceleration;
	_previousAcceleration.AddScaledVector(_forceAccumulator, _inverseMass);

	//Calculate angular acceleration from torque forces
	Vector3 angularAcceleration = _inverseInertiaTensorWorld.Transform(_torqueAccumulator);

	//Calculate Velocity and rotation changes
	_velocity += _previousAcceleration * dt;
	_rotation += angularAcceleration * dt;

	//Applies damping to velocity and rotation
	_velocity *= powf(_linearDamping, dt);
	_rotation *= powf(_angularDamping, dt);

	//Updates position
	_position.AddScaledVector(_velocity, dt);

	//Updates Orientation
	_orientation.AddScaledVector(_rotation, dt);

	//Clear the force accumulators
	EndUpdate(_velocity.ScalarProduct(_velocity) + _rotation.ScalarProduct(_rotation), dt);
}

void RigidbodyComponent::Initialize()
{
	PhysicsModelComponent::Initialize();

	//Sets the position and rotation for the rigidbody based on our transform components
	SetPosition(_pTransformComponent->GetPosition());
	SetRotation(_pTransformComponent->GetRotation());

	//Sets the Rigidbody to being able to sleep and not being awake by default
	SetCanSleep(true);
	SetAwake(false);
	SetAngularDamping(0.8f);
	SetLinearDamping(0.95f);

	//Assumes this is a cube by default
	SetCubeInertiaTensor();
	
	//Clears accumulator
	ClearAccumulator();

	//Calculates the transform, orientation etc to make sure there will be no first frame glitches
	CalculateDerivedData();
}

