#include "Level5.h"

void Level5::LoadLevel()
{
	Scene::LoadLevel();

	//Initializes the gravity generator 
	if (_pGravityGenerator == nullptr) _pGravityGenerator = std::make_unique<GravityForceGenerator>(Vector3(0.0f, -9.81f, 0.0f));

	//Initializes the Rigidbodies if they are null, if not then change the owner of them to the new objects
	if (_pBottomRb == nullptr) _pBottomRb = std::make_unique<RigidbodyComponent>(_pGameObjects[0]);
	else _pBottomRb->SetOwner(_pGameObjects[0]);

	if (_pTopRb == nullptr) _pTopRb = std::make_unique<RigidbodyComponent>(_pGameObjects[1]);
	else _pTopRb->SetOwner(_pGameObjects[1]);

	//Create the colliders
	if (_pTopCube == nullptr) _pTopCube = std::make_unique<Box>(_pTopRb.get());
	if (_pBottomCube == nullptr) _pBottomCube = std::make_unique<Box>(_pBottomRb.get());
	if (_pGround == nullptr) _pGround = std::make_unique<CollisionPlane>(Vector3(0, 1, 0), 0.0f);

	//Initializes the contact resolver with suitable values
	if (_pContactResolver == nullptr) _pContactResolver = std::make_unique<ContactResolver>(LEVEL_5_MAX_CONTACTS, 0.01f, 0.03f);

	//Sets up the contact data variable
	_contactData._contactArray = _contactsArray;
	_contactData._friction = 0.9f;
	_contactData._restitution = 0.1f;
	_contactData._tolerance = 0.1f;

	LoadGround();
}


void Level5::Update(float dt)
{
	//Apply gravity to our objects
	_pGravityGenerator->Update(_pTopRb.get(), dt);
	_pGravityGenerator->Update(_pBottomRb.get(), dt);

	//Calculate the colliders transforms
	_pTopCube->CalculateInternals();
	_pBottomCube->CalculateInternals();

	//Reset our contact count
	_contactData.Reset(LEVEL_5_MAX_CONTACTS);

	//Check for collisions
	CollisionDetector::BoxAndPlane(*_pTopCube, *_pGround, &_contactData);
	CollisionDetector::BoxAndPlane(*_pBottomCube, *_pGround, &_contactData);
	CollisionDetector::BoxAndBox(*_pTopCube, *_pBottomCube, &_contactData);

	//Resolves the collisions
	_pContactResolver->ResolveContacts(_contactData._contactArray, _contactData._contactCount, dt);
}


void Level5::DrawUI()
{
	ImGui::Begin("Test 5");

	ImGui::Text("Test 5 is showcasing collision response between two rigidbody objects, both of these use box colliders");

	ImGui::Text("Contacts this frame: %d", _contactData._contactCount);

	ResetButton();

	ImGui::End();
}
