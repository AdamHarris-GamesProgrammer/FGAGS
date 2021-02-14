#pragma once

#include "Vector3.h"
#include <assert.h>
#include "Transform.h"
#include "PhysicsModel.h"

class Particle : public PhysicsModel
{
public:
	void Update(float dt) override;
};

