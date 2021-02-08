#pragma once
#include "Rigidbody.h"
#include "Precision.h"

class ForceGenerator
{
public:
	virtual void Update(Rigidbody* body, real dt) = 0;
};

