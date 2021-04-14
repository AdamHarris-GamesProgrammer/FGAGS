#pragma once
#include "Vector3.h"
#include "Component.h"
#include "ComponentIDs.h"
#include "Transform.h"
#include "Object.h"

class PhysicsModelComponent : public Component
{
public:
	PhysicsModelComponent(Object* owner) : Component(owner, PhysicsModel, 50.0f)
	{
		Initialize();
	}
	PhysicsModelComponent(Object* owner, ComponentID id, float updatePriority) : Component(owner, id, updatePriority)
	{
		Initialize();
	}

	void SetOwner(Object* newOwner) override {
		Component::SetOwner(newOwner);
		Initialize();
	}

	virtual void AddForce(const Vector3& force);


	bool BeginUpdate(float dt);
	virtual void Update(float dt) = 0;
	virtual void EndUpdate(float currMot, float dt);


	virtual void ClearAccumulator();
#pragma region getters
	float GetMass() const;
	float GetDamping() const;
	float GetInverseMass() const;
	Vector3 GetAcceleration() const;
	Vector3 GetVelocity() const;
	Vector3 GetPosition() const;
	bool GetAwake() const;
	float GetSleepEpsilon();

	bool HasFiniteMass() const;
#pragma endregion

#pragma region Setters
	void SetInverseMass(float val);
	void SetDamping(float val);
	void SetAcceleration(Vector3 val = Vector3());
	void SetAcceleration(float x, float y, float z);
	void SetVelocity(Vector3 val = Vector3());
	void SetVelocity(float x, float y, float z);
	void SetPosition(Vector3 val = Vector3());
	void SetPosition(float x, float y, float z);
	void SetMass(const float mass);
	void SetSleepEpsilon(float value) { _sleepEpsilon = value; }
	virtual void SetAwake(const bool awake = true);
	void SetCanSleep(const bool canSleep);
#pragma endregion
protected:
	Vector3 _position;
	Vector3 _velocity;
	Vector3 _acceleration;

	Vector3 _forceAccumulator;

	float _inverseMass;
	float _linearDamping;

	bool _isAwake;
	float _motion;
	bool _canSleep;

	float _sleepEpsilon = 0.1;

	virtual void Initialize();

	void CheckSleep(float currMot, float dt);

	TransformComponent* _pTransformComponent;
};

