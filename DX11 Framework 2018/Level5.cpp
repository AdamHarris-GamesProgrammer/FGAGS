#include "Level5.h"

void Level5::LoadLevel()
{
	Level::LoadLevel();

	_pGravityGenerator = new GravityForceGenerator(Vector3(0.0f, -9.81f, 0.0f));

	_pBottomRb = new RigidbodyComponent(_pGameObjects[0]);
	_pBottomRb->SetCubeInertiaTensor();

	_pBottomCube = new Box(_pBottomRb);

	_pTopRb = new RigidbodyComponent(_pGameObjects[1]);
	_pTopRb->SetCubeInertiaTensor();

	_pTopCube = new Box(_pTopRb);

	_pBottomRb->SetAwake();
	_pTopRb->SetAwake();

	_pGround = new CollisionPlane(Vector3(0, 1, 0), 0.0f);

	_pContactResolver = new ContactResolver(MAX_CONTACTS);
	_contactData._contactArray = _contactsArray;
	_contactData._friction = 0.9;
	_contactData._restitution = 0.1;
	_contactData._tolerance = 0.1;

	LoadGround();
}

void Level5::ExitLevel()
{
	delete _pBottomCube;
	delete _pTopCube;
	delete _pGround;
	delete _pContactResolver;

	_pBottomCube = nullptr;
	_pTopCube = nullptr;
	_pGround = nullptr;
	_pContactResolver = nullptr;

	_pCameras.clear();
	
}

void Level5::Update(float dt)
{
	_pGravityGenerator->Update(_pTopRb,dt);
	_pGravityGenerator->Update(_pBottomRb,dt);


	_pTopCube->CalculateInternals();
	_pBottomCube->CalculateInternals();

	_contactData.Reset(MAX_CONTACTS);
	_contactData._friction = (float)0.9;
	_contactData._restitution = (float)0.1;
	_contactData._tolerance = (float)0.1;

	if (_contactData.HasMoreContacts()) {
		CollisionDetector::BoxAndHalfSpace(*_pTopCube, *_pGround, &_contactData);
		CollisionDetector::BoxAndHalfSpace(*_pBottomCube, *_pGround, &_contactData);
		CollisionDetector::BoxAndBox(*_pTopCube, *_pBottomCube, &_contactData);

		_pContactResolver->ResolveContacts(_contactData._contactArray, _contactData._contactCount, dt);
	}

}


void Level5::DrawUI()
{
	ImGui::Begin("Test 5");

	ImGui::Text("Contacts this frame: %d", _contactData._contactCount);

	ResetButton();

	ImGui::End();
}
