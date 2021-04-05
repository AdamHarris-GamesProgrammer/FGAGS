#pragma once
#include "Level.h"
#include "CollisionPrimitives.h"
#include "Plane.h"
#include "GravityForceGenerator.h"

#define MAX_CONTACTS 10

class Level5 :  public Level
{
public:
	Level5(Graphics* gfx, const char* levelName) : Level(gfx, levelName) {}


	void Update(float dt) override;
	void LoadLevel() override;

private:
	void DrawUI() override;


	std::unique_ptr<GravityForceGenerator> _pGravityGenerator;

	Contact _contactsArray[MAX_CONTACTS];
	CollisionData _contactData;
	std::unique_ptr<ContactResolver> _pContactResolver;

	std::unique_ptr<Box> _pBottomCube;
	std::unique_ptr<Box> _pTopCube;
	std::unique_ptr<CollisionPlane> _pGround;

	std::unique_ptr<RigidbodyComponent> _pTopRb;
	std::unique_ptr<RigidbodyComponent> _pBottomRb;
};

