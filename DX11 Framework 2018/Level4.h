#pragma once
#include <memory>

#include "Scene.h"
#include "GravityForceGenerator.h"

#include "ContactResolver.h"
#include "CollisionPrimitives.h"
#include "CollisionDetector.h"
#include "CollisionData.h"

//Stores our maxinum number of contacts per frame
#define LEVEL_4_MAX_CONTACTS 10
class Level4 : public Scene
{
public:
	Level4(std::shared_ptr<Graphics> gfx, const char* filename) : Scene(gfx, filename) {}

	void LoadLevel() override;



	void Update(float dt) override;

private:
	void DrawUI() override;

private:
	//Using unique pointers automatically handle deallocation
	std::unique_ptr<GravityForceGenerator> _pGravityGenerator;

	//Creates a array of contacts
	Contact _contactsArray[LEVEL_4_MAX_CONTACTS];
	CollisionData _contactData;

	//Contact resolver is responsible for responding to collisions
	std::unique_ptr<ContactResolver> _pContactResolver;

	//The colliders for this level
	std::unique_ptr<Box> _pBottomCube;
	std::unique_ptr<Box> _pTopCube;
	std::unique_ptr<CollisionPlane> _pGround;

	//The rigidbodies for this level
	std::unique_ptr<RigidbodyComponent> _pTopRb;
	std::unique_ptr<RigidbodyComponent> _pBottomRb;

};

