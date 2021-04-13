#pragma once

#include "Vector3.h"
#include <assert.h>
#include "Transform.h"
#include "PhysicsModel.h"

class ParticleComponent : public PhysicsModelComponent
{
public:
	ParticleComponent(Object* owner) : PhysicsModelComponent(owner, Particle, 50.0f) 
	{
		Initialize();
	}

	void Update(float dt) override;
};

