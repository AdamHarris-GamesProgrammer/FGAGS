#include "Level6.h"

void Level6::LoadLevel()
{
	Scene::LoadLevel();

	if (_pGravityGenerator == nullptr) {
		_pGravityGenerator = std::make_unique<GravityForceGenerator>(Vector3(0.0f, -9.81f, 0.0f));
	}

	if (_pSphereRb == nullptr) {
		_pSphereRb = std::make_unique<RigidbodyComponent>(_pGameObjects[0]);
	}
	else
	{
		_pSphereRb->SetOwner(_pGameObjects[0]);
	}

	_pSphereRb->SetAwake();
	_pSphereRb->SetMass(20.0f);
	_pSphereRb->SetSphereInertiaTensor();

	if (_pSphereCollider == nullptr) {
		_pSphereCollider = std::make_unique<Sphere>(_pSphereRb.get());
	}
	else
	{
		_pSphereCollider->_body = _pSphereRb.get();
	}

	if (_pCubeRb == nullptr) {
		_pCubeRb = std::make_unique<RigidbodyComponent>(_pGameObjects[1]);
	}
	else
	{
		_pCubeRb->SetOwner(_pGameObjects[1]);
	}

	_pCubeRb->SetAwake();
	_pCubeRb->SetCubeInertiaTensor();

	if (_pBoxCollider == nullptr) {
		_pBoxCollider = std::make_unique<Box>(_pCubeRb.get(), Vector3(1.0, 1.0, 1.0));
	}
	else
	{
		_pBoxCollider->_body = _pCubeRb.get();
	}

	if (_pGroundCollider == nullptr) {
		_pGroundCollider = std::make_unique<CollisionPlane>(Vector3(0, 1, 0), 0.0f);
	}

	if (_pContactResolver == nullptr) {
		_pContactResolver = std::make_unique<ContactResolver>(LEVEL_6_MAX_CONTACTS);
	}

	_contactData._contactArray = _contactsArray;
	_contactData._friction = 0.9;
	_contactData._restitution = 0.1;
	_contactData._tolerance = 0.1;

	LoadGround();
}

void Level6::Update(float dt)
{
	_pGravityGenerator->Update(_pCubeRb.get(), dt);
	_pGravityGenerator->Update(_pSphereRb.get(), dt);

	_pSphereCollider->CalculateInternals();
	_pBoxCollider->CalculateInternals();

	_contactData.Reset(LEVEL_6_MAX_CONTACTS);

	if (_contactData.HasMoreContacts()) {
		CollisionDetector::BoxAndSphere(*_pBoxCollider, *_pSphereCollider, &_contactData);
		CollisionDetector::SphereAndTruePlane(*_pSphereCollider, *_pGroundCollider, &_contactData);
		CollisionDetector::BoxAndHalfSpace(*_pBoxCollider, *_pGroundCollider, &_contactData);

		_pContactResolver->ResolveContacts(_contactData._contactArray, _contactData._contactCount, dt);
	}
}




void Level6::DrawUI()
{
	ImGui::Begin("Test 6");

	ImGui::Text("Contacts this frame: %d", _contactData._contactCount);

	ResetButton();

	ImGui::End();
}
