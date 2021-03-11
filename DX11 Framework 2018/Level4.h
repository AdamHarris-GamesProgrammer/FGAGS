#pragma once
#include "Level.h"

#define MAX_CONTACTS 10
class Level4 : public Level
{
public:
	Level4(Graphics* gfx, const char* filename) : Level(gfx, filename) {
		LoadLevel();
	}

	void LoadLevel() override;


	void ExitLevel() override;


	void PollInput(float dt) override;


	void Update(float dt) override;


	void Reset() override;

private:
	void DrawUI() override;

private:
	Contact _contactsArray[MAX_CONTACTS];
	CollisionData _contactData;
	ContactResolver* _contactResolver;

	Box* _pBottomCube;
	Box* _pTopCube;
	CollisionPlane* _pGround;

};

