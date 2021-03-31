#pragma once
#include "ForceGenerator.h"

class RestingForceGenerator : public ForceGenerator {
public:
	RestingForceGenerator(float yVal = 1.0f) : _yVal(yVal) {

	}

	void Update(RigidbodyComponent* body, float dt) override
	{
		//Touching the floor
		if (body->GetPosition().y <= _yVal) {
			body->ClearAccumulator();
			body->SetVelocity(Vector3());
			body->SetAcceleration(Vector3());
		}
	}

private:
	float _yVal;

};