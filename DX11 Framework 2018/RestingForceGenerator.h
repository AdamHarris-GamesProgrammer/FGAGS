#pragma once
#include "ForceGenerator.h"

class RestingForceGenerator {
public:
	RestingForceGenerator(float yVal = 1.0f) : _yVal(yVal) {

	}

	void Update(PhysicsModelComponent* body, float dt)
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