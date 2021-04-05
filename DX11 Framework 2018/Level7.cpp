#include "Level7.h"

void Level7::LoadLevel()
{
	Level::LoadLevel();

	_pGravityGenerator = new GravityForceGenerator(Vector3(0.0f, -9.81f, 0.0f));

	_pPlayerRb = new RigidbodyComponent(_pGameObjects[0]);
	_pPlayerRb->SetAwake();
	_pPlayerRb->SetMass(20.0f);
	_pSphereCollider = new Sphere(_pPlayerRb);

	_pCubeRb = new RigidbodyComponent(_pGameObjects[1]);
	_pCubeRb->SetAwake();
	_pCubeRb->SetMass(0.1f);
	_pBoxCollider = new Box(_pCubeRb, Vector3(1.0, 1.0, 1.0));


	_pGroundCollider = new CollisionPlane(Vector3(0, 1, 0), 0.0f);

	_pContactResolver = new ContactResolver(MAX_CONTACTS);
	_contactData._contactArray = _contactsArray;
	_contactData._friction = 0.9;
	_contactData._restitution = 0.1;
	_contactData._tolerance = 0.1;

	LoadGround();
}

void Level7::Update(float dt)
{
	_pGravityGenerator->Update(_pCubeRb, dt);
	_pGravityGenerator->Update(_pPlayerRb, dt);

	_pSphereCollider->CalculateInternals();
	_pBoxCollider->CalculateInternals();

	_contactData.Reset(MAX_CONTACTS);
	_contactData._friction = (float)0.9;
	_contactData._restitution = (float)0.1;
	_contactData._tolerance = (float)0.1;

	if (_contactData.HasMoreContacts()) {
		CollisionDetector::BoxAndSphere(*_pBoxCollider, *_pSphereCollider, &_contactData);
		CollisionDetector::SphereAndTruePlane(*_pSphereCollider, *_pGroundCollider, &_contactData);
		CollisionDetector::BoxAndHalfSpace(*_pBoxCollider, *_pGroundCollider, &_contactData);

		_pContactResolver->ResolveContacts(_contactData._contactArray, _contactData._contactCount, dt);
	}

	_pCurrentCamera->GetTransform().SetPosition(_pPlayerRb->GetPosition() + _cameraOffset);
}

void Level7::PollInput(float dt)
{
	if (GetAsyncKeyState('W')) {
		_pPlayerRb->AddForce(Vector3(0.0f, 0.0f, 10.0f) * _movementPower);
	}
	else if (GetAsyncKeyState('S')) {
		_pPlayerRb->AddForce(Vector3(0.0f, 0.0f, -10.0f) * _movementPower);
	}
	if (GetAsyncKeyState('A')) {
		_pPlayerRb->AddForce(Vector3(-10.0f, 0.0f, 0.0f) * _movementPower);
	}
	else if (GetAsyncKeyState('D')) {
		_pPlayerRb->AddForce(Vector3(10.0f, 0.0f, 0.0f) * _movementPower);
	}
}

void Level7::DrawUI()
{
	ImGui::Begin("Test 7");




	ResetButton();


	ImGui::End();
}
