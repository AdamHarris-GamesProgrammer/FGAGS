#pragma once
#include "Level.h"
#include "CollisionPrimitives.h"
#include "Plane.h"

#define MAX_CONTACTS 10

class Level5 :  public Level
{
public:
	Level5(Graphics* gfx, const char* levelName) : Level(gfx, levelName) 
	{
		LoadLevel();
	}

	void Reset() override;

	void ExitLevel() override;

	void PollInput(float dt) override;

	void Update(float dt) override;
	void LoadLevel() override;

private:
	Contact _contactsArray[MAX_CONTACTS];
	CollisionData _contactData;
	ContactResolver* _pContactResolver;

	Box* _pBottomCube;
	Box* _pTopCube;
	CollisionPlane* _pGround;

	void DrawUI() override;

};

