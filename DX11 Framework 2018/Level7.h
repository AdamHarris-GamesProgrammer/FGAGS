#pragma once
#include "Level.h"
#include <memory>
#include "Rigidbody.h"
#include "GravityForceGenerator.h"

#define MAX_CONTACTS 40
class Level7 : public Level
{
public:
	Level7(Graphics* gfx, const char* levelName) : Level(gfx, levelName) {
		LoadLevel();
	}

	void LoadLevel() override;


	void Update(float dt) override;
	void PollInput(float dt) override;
private:
	void DrawUI() override;

	Contact _contactsArray[MAX_CONTACTS];
	CollisionData _contactData;
	ContactResolver* _pContactResolver;

	CollisionPlane* _pGroundCollider;
	Sphere* _pSphereCollider;
	Box* _pBoxCollider;

	RigidbodyComponent* _pPlayerRb;
	RigidbodyComponent* _pCubeRb;
	GravityForceGenerator* _pGravityGenerator;

	float _movementPower = 10.0f;
	Vector3 _cameraOffset = Vector3(0.0f, 5.0f, -15.0f);

};

