#pragma once
#include "Scene.h"
#include "GravityForceGenerator.h"

#include "ContactResolver.h"
#include "CollisionPrimitives.h"
#include "CollisionDetector.h"
#include "CollisionData.h"

//Defines the maxinum amount of contacts per frame 
#define LEVEL_5_MAX_CONTACTS 10
class Level5 :  public Scene
{
public:
	Level5(Graphics* gfx, const char* levelName) : Scene(gfx, levelName) {}


	void Update(float dt) override;
	void LoadLevel() override;

private:
	void DrawUI() override;

	//Using unique pointers to automatically deallocate memory

	//Using a gravity force generator to force the cubes downward onto the collision plane and onto each other
	std::unique_ptr<GravityForceGenerator> _pGravityGenerator;

	//Creates the contact array
	Contact _contactsArray[LEVEL_5_MAX_CONTACTS];

	//Stores the collision data for this frame
	CollisionData _contactData;

	//Contact resolver is used to resolve contacts this frame
	std::unique_ptr<ContactResolver> _pContactResolver;

	//Colliders for the scene
	std::unique_ptr<Box> _pBottomCube;
	std::unique_ptr<Box> _pTopCube;
	std::unique_ptr<CollisionPlane> _pGround;

	//Rigidbody components for the scene 
	std::unique_ptr<RigidbodyComponent> _pTopRb;
	std::unique_ptr<RigidbodyComponent> _pBottomRb;
};

