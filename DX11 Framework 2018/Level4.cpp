#include "Level4.h"

void Level4::LoadLevel()
{
	Scene::LoadLevel();

	if (_pGravityGenerator == nullptr)
		_pGravityGenerator = std::make_unique<GravityForceGenerator>(Vector3(0.0f, -9.81f, 0.0f));

	//Initializes the rigidbody components in this level and changes the owners if reloading the scene
	if (_pBottomRb == nullptr) _pBottomRb = std::make_unique<RigidbodyComponent>(_pGameObjects[0]);
	else _pBottomRb->SetOwner(_pGameObjects[0]);
	if (_pTopRb == nullptr) _pTopRb = std::make_unique<RigidbodyComponent>(_pGameObjects[1]);
	else _pTopRb->SetOwner(_pGameObjects[1]);

	//Initializes colliders in this level
	if (_pTopCube == nullptr) _pTopCube = std::make_unique<Box>(_pTopRb.get());
	if (_pBottomCube == nullptr) _pBottomCube = std::make_unique<Box>(_pBottomRb.get());
	if (_pGround == nullptr) _pGround = std::make_unique<CollisionPlane>(Vector3(0, 1, 0), 0.0f);

	//Initializes the contact resolver for this level
	if (_pContactResolver == nullptr) _pContactResolver = std::make_unique<ContactResolver>(LEVEL_4_MAX_CONTACTS, 0.01f, 0.03f);

	//Sets up the contact data for this
	_contactData._contactArray = _contactsArray;
	_contactData._friction = 0.9f;
	_contactData._restitution = 0.1f;
	_contactData._tolerance = 0.1f;

	LoadGround();
}

void Level4::Update(float dt)
{
	//Applies gravity to our objects
	_pGravityGenerator->Update(_pTopRb.get(), dt);
	_pGravityGenerator->Update(_pBottomRb.get(), dt);

	//Calculates the collider transforms
	_pTopCube->CalculateInternals();
	_pBottomCube->CalculateInternals();

	//Reset the contacts saved
	_contactData.Reset(LEVEL_4_MAX_CONTACTS);

	//Detects collisions
	CollisionDetector::BoxAndPlane(*_pTopCube, *_pGround, &_contactData);
	CollisionDetector::BoxAndPlane(*_pBottomCube, *_pGround, &_contactData);
	CollisionDetector::BoxAndBox(*_pTopCube, *_pBottomCube, &_contactData);

	//Resolve contacts this frame
	_pContactResolver->ResolveContacts(_contactData._contactArray, _contactData._contactCount, dt);
}

void Level4::DrawUI()
{
	ImGui::Begin("Test 4");

	ImGui::Text("This test showcases collision detection between two rigidbody objects and the floor");
	ImGui::Text("There is also collision response but it is not the focus of this test");

	ImGui::Text("Contacts this frame: %d", _contactData._contactCount);

	ResetButton();

	ImGui::End();
}
