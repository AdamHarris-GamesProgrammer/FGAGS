#pragma once

#include "Vector3.h"
#include "Precision.h"
#include <assert.h>
#include "Transform.h"
#include "PhysicsModel.h"

class Particle : public PhysicsModel
{
public:
	void Update(real dt) override;
};

