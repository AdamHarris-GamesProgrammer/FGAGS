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


	void SetSleepEpsilon(float value) { _sleepEpsilon = value; }

	float GetSleepEpsilon() { return _sleepEpsilon; }
	bool GetAwake() const {
		return _isAwake;
	}

	 virtual void SetAwake(const bool awake = true) {
		if (awake) {
			_isAwake = true;
			_motion = _sleepEpsilon * 2.0f;
		}
		else
		{
			_isAwake = false;
			_velocity.Zero();
		}
	}

	void SetCanSleep(const bool canSleep) {
		_canSleep = canSleep;

		if (!canSleep && !_isAwake) SetAwake();
	}

private:
	

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

	void CheckSleep(float currMot, float dt) {
		if (_canSleep) {
			float currentMotion = currMot;
			float bias = powf(0.5, dt);

			_motion = bias * _motion + (1 - bias) * currentMotion;

			if (_motion < _sleepEpsilon) SetAwake(false);
			else if (_motion > 10 * _sleepEpsilon) _motion = 10 * _sleepEpsilon;
		}
	}

	TransformComponent* _pTransformComponent;
};

