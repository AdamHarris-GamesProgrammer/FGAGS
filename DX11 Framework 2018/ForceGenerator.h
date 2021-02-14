#pragma once
#include "Rigidbody.h"
#include "Precision.h"

class ForceGenerator
{
public:
	virtual void Update(Rigidbody* body, float dt) = 0;
};

