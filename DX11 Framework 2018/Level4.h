#pragma once
#include "Level.h"
#include "GravityForceGenerator.h"
#include <memory>

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

