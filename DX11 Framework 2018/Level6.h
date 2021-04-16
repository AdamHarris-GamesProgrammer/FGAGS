#pragma once
#include "Scene.h"
#include "GravityForceGenerator.h"
#include <memory>

#include "CollisionData.h"
#include "CollisionDetector.h"
#include "CollisionPrimitives.h"
#include "ContactResolver.h"

//Stores the maximum amount of contacts for this level
#define LEVEL_6_MAX_CONTACTS 10
class Level6 : public Scene
{
public:
	Level6(std::shared_ptr<Graphics> gfx, const char* levelName) : Scene(gfx, levelName) {}

	void Update(float dt) override;

	void LoadLevel() override;

private:
	void DrawUI() override;

	//contact array
	Contact _contactsArray[LEVEL_6_MAX_CONTACTS];

	//collision data
	CollisionData _contactData;

	//Using unique ptr to handle memory deallocation
	//The contact resolver handles adjusting positions and velocities of a object when it collides
	std::unique_ptr<ContactResolver> _pContactResolver;

	//Colliders for the level
	std::unique_ptr<CollisionPlane> _pGroundCollider;
	std::unique_ptr<Sphere> _pSphereCollider;
	std::unique_ptr<Box> _pBoxCollider;

	//Rigidbody components for the level
	std::unique_ptr<RigidbodyComponent> _pSphereRb;
	std::unique_ptr<RigidbodyComponent> _pCubeRb;

	//Gravity generator for the level
	std::unique_ptr<GravityForceGenerator> _pGravityGenerator;
};

