#pragma once
#include "Level.h"
#include "CollisionPrimitives.h"
#include "Plane.h"
#include "GravityForceGenerator.h"

#define MAX_CONTACTS 20
class Level6 : public Level
{
public:
	Level6(Graphics* gfx, const char* levelName) : Level(gfx, levelName) {}

	void ExitLevel() override;

	void Update(float dt) override;

	void LoadLevel() override;

private:
	void DrawUI() override;

	Contact _contactsArray[MAX_CONTACTS];
	CollisionData _contactData;
	ContactResolver* _pContactResolver;

	CollisionPlane* _pGroundCollider;
	Sphere* _pSphereCollider;
	Box* _pBoxCollider;

	RigidbodyComponent* _pSphereRb;
	RigidbodyComponent* _pCubeRb;
	GravityForceGenerator* _pGravityGenerator;
};

