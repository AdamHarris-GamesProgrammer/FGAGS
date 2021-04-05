#pragma once
#include "Level.h"
#include "CollisionPrimitives.h"
#include "Plane.h"
#include "GravityForceGenerator.h"
#include <memory>

#define MAX_CONTACTS 20
class Level6 : public Level
{
public:
	Level6(Graphics* gfx, const char* levelName) : Level(gfx, levelName) {}

	void Update(float dt) override;

	void LoadLevel() override;

private:
	void DrawUI() override;

	Contact _contactsArray[MAX_CONTACTS];
	CollisionData _contactData;
	std::unique_ptr<ContactResolver> _pContactResolver;

	std::unique_ptr<CollisionPlane> _pGroundCollider;
	std::unique_ptr<Sphere> _pSphereCollider;
	std::unique_ptr<Box> _pBoxCollider;

	std::unique_ptr<RigidbodyComponent> _pSphereRb;
	std::unique_ptr<RigidbodyComponent> _pCubeRb;
	std::unique_ptr<GravityForceGenerator> _pGravityGenerator;

	
};

