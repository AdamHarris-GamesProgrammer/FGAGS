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

	//Used for checking if we need to update physics
	bool BeginUpdate(float dt);

	//Updates our physics system
	virtual void Update(float dt) = 0;

	//Ends the update and decides if objects need to sleep
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

	//force accumulator acts as a implementation of dAlembert's principle
	Vector3 _forceAccumulator;

	//Holds the mass
	float _inverseMass;

	//Damping, acts as a simplistic drag
	float _linearDamping;

	//Stores the physics systems sleep values
	bool _isAwake;
	bool _canSleep;

	//Motion is used to decide if a object should sleep
	float _motion;

	//Epsilon for sleeping
	float _sleepEpsilon = 0.1f;

	//Initialization method for setting up initial values
	virtual void Initialize();

	//Checks if the physics can sleep based on a motion vector which is passed in
	void CheckSleep(float currMot, float dt);

	//All physics models need a pointer to the objects transform component in order to manipulate positional data
	TransformComponent* _pTransformComponent;
};

