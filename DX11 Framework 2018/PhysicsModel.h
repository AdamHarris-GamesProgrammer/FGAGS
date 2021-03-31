#pragma once
#include "Vector3.h"
#include "Component.h"
#include "ComponentIDs.h"

class PhysicsModelComponent : public Component
{
public:
	PhysicsModelComponent(Object* owner) : Component(owner, PhysicsModel, 50.0f) {}
	PhysicsModelComponent(Object* owner, ComponentID id, float updatePriority) : Component(owner, id, updatePriority) {}

	virtual void AddForce(const Vector3& force);
	virtual void Update(float dt) = 0;
	virtual void ClearAccumulator();

	float GetMass() const;
	void SetMass(const float mass);
	bool HasFiniteMass() const {
		return _inverseMass >= 0.0f;
	}

	float GetInverseMass() const;
	void SetInverseMass(float val);

	float GetDamping() const;
	void SetDamping(float val);

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

	float _inverseMass;
	float _linearDamping;
};

