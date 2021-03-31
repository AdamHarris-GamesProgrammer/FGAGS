#pragma once
#include "Rigidbody.h"

class ForceGenerator
{
public:
	virtual void Update(RigidbodyComponent* body, float dt) = 0;
};

