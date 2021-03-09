#pragma once
#include "Rigidbody.h"

class ForceGenerator
{
public:
	virtual void Update(Rigidbody* body, float dt) = 0;
};

