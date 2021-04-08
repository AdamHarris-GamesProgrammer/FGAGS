#include "Level4.h"

void Level4::LoadLevel()
{
	Scene::LoadLevel();

	if (_pGravityGenerator == nullptr) {
		_pGravityGenerator = std::make_unique<GravityForceGenerator>(Vector3(0.0f, -9.81f, 0.0f));
	}

	if (_pBottomRb == nullptr) {
		_pBottomRb = std::make_unique<RigidbodyComponent>(_pGameObjects[0]);
		_pBottomRb->SetAwake();
	}
	else
	{
		_pBottomRb->SetOwner(_pGameObjects[0]);
	}
	_pBottomRb->SetCubeInertiaTensor();
	
	if (_pTopRb == nullptr) {
		_pTopRb = std::make_unique<RigidbodyComponent>(_pGameObjects[1]);
		_pTopRb->SetAwake();
	}
	else
	{
		_pTopRb->SetOwner(_pGameObjects[1]);
	}

	if (_pTopCube == nullptr) {
		_pTopCube = std::make_unique<Box>(_pTopRb.get());
	}
	else
	{
		_pTopCube->_body = _pTopRb.get();
	}


	if (_pBottomCube == nullptr) {
		_pBottomCube = std::make_unique<Box>(_pBottomRb.get());
	}
	else
	{
		_pBottomCube->_body = _pBottomRb.get();
	}

	if (_pGround == nullptr) {
		_pGround = std::make_unique<CollisionPlane>(Vector3(0, 1, 0), 0.0f);
	}

	if (_pContactResolver == nullptr) {
		_pContactResolver = std::make_unique<ContactResolver>(MAX_CONTACTS);
	}

	_contactData._contactArray = _contactsArray;
	_contactData._friction = 0.9f;
	_contactData._restitution = 0.1f;
	_contactData._tolerance = 0.1f;

	LoadGround();
}

void Level4::ExitLevel()
{
	_pCameras.clear();
}

void Level4::Update(float dt)
{
	_pGravityGenerator->Update(_pTopRb.get(), dt);
	_pGravityGenerator->Update(_pBottomRb.get(), dt);

	_pTopCube->CalculateInternals();
	_pBottomCube->CalculateInternals();

	_contactData.Reset(MAX_CONTACTS);

	if (_contactData.HasMoreContacts()) {
		CollisionDetector::BoxAndHalfSpace(*_pTopCube, *_pGround, &_contactData);
		CollisionDetector::BoxAndHalfSpace(*_pBottomCube, *_pGround, &_contactData);
		CollisionDetector::BoxAndBox(*_pTopCube, *_pBottomCube, &_contactData);

		_pContactResolver->ResolveContacts(_contactData._contactArray, _contactData._contactCount, dt);
	}
}

void Level4::DrawUI()
{
	ImGui::Begin("Test 4");
	
	ImGui::Text("Contacts this frame: %d", _contactData._contactCount);
	
	ResetButton();

	ImGui::End();
}
