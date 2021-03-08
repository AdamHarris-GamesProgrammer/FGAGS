#pragma once
#include "Level.h"
#include "CollisionPrimitives.h"
#include "Plane.h"

#define MAX_CONTACTS 10

class Level1 :  public Level
{
public:
	Level1(Graphics* gfx, const char* levelName) : Level(gfx, levelName) 
	{
		LoadLevel();
	}

	void Reset() override;

	void PollInput(float dt) override;

	void Update(float dt) override;
	void LoadLevel() override;

private:
	Contact contacts[MAX_CONTACTS];
	CollisionData cData;
	ContactResolver* cResolver;

	Plane* pGroundPlane = nullptr;

	Box* pBottomCube;
	Box* pTopCube;
	CollisionPlane* pGround;
};

