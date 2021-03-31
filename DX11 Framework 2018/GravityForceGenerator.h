#pragma once
#include "ForceGenerator.h"

class GravityForceGenerator : public ForceGenerator {
public:
	GravityForceGenerator(const Vector3& gravity) {
		_gravity = gravity;
	}

	virtual void Update(RigidbodyComponent* body, float dt) {
		if (!body->HasFiniteMass()) return;

		body->AddForce(_gravity * body->GetMass());
	}

	void SetGravity(Vector3 val) {
		_gravity = val;
	}

	Vector3 GetGravity() const {
		return _gravity;
	}
private:
	Vector3 _gravity;

};
