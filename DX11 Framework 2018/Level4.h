#pragma once
#include "Level.h"
#include "GravityForceGenerator.h"

#define MAX_CONTACTS 10
class Level4 : public Level
{
public:
	Level4(Graphics* gfx, const char* filename) : Level(gfx, filename) {}

	void LoadLevel() override;


	void ExitLevel() override;

	void Update(float dt) override;

private:
	void DrawUI() override;

private:
	GravityForceGenerator* _pGravityGenerator;

	Contact _contactsArray[MAX_CONTACTS];
	CollisionData _contactData;
	ContactResolver* _pContactResolver;

	Box* _pBottomCube;
	Box* _pTopCube;
	CollisionPlane* _pGround;

	RigidbodyComponent* _pTopRb;
	RigidbodyComponent* _pBottomRb;

};

