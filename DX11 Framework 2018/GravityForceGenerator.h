#pragma once
#include "ForceGenerator.h"

class GravityForceGenerator : public ForceGenerator {
public:
	GravityForceGenerator(const Vector3& gravity) {
		_gravity = gravity;
	}

	virtual void Update(Rigidbody* body, float dt) {
		if (!body->HasFiniteMass()) return;

		body->AddForce(_gravity * body->GetMass());
	}

private:
	Vector3 _gravity;

};
