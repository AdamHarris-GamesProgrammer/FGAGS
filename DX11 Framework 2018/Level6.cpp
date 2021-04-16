#include "Level6.h"

void Level6::LoadLevel()
{
	Scene::LoadLevel();

	//Initializes gravity generator
	if (_pGravityGenerator == nullptr)
		_pGravityGenerator = std::make_unique<GravityForceGenerator>(Vector3(0.0f, -9.81f, 0.0f));

	//Creates the rigidbody components for the level
	if (_pSphereRb == nullptr) _pSphereRb = std::make_unique<RigidbodyComponent>(_pGameObjects[0]);
	else _pSphereRb->SetOwner(_pGameObjects[0]);
	if (_pCubeRb == nullptr) _pCubeRb = std::make_unique<RigidbodyComponent>(_pGameObjects[1]);
	else _pCubeRb->SetOwner(_pGameObjects[1]);

	//The inertia tensor by default is for a cube, the sphere inertia tensor is calculated differently
	_pSphereRb->SetSphereInertiaTensor();

	//Creates the collider objects
	if (_pSphereCollider == nullptr)  _pSphereCollider = std::make_unique<Sphere>(_pSphereRb.get());
	if (_pBoxCollider == nullptr) _pBoxCollider = std::make_unique<Box>(_pCubeRb.get(), Vector3(1.0, 1.0, 1.0));
	if (_pGroundCollider == nullptr) _pGroundCollider = std::make_unique<CollisionPlane>(Vector3(0, 1, 0), 0.0f);


	//passing through epsilons of 0.01f (velocity, default param) and 0.03f (position) using a higher position epsilon to reduce shakey collisions
	if (_pContactResolver == nullptr)
		_pContactResolver = std::make_unique<ContactResolver>(LEVEL_6_MAX_CONTACTS, 0.01f, 0.03f);

	//Initializes our contact data to suitable values
	_contactData._contactArray = _contactsArray;
	_contactData._friction = 0.9f;
	_contactData._restitution = 0.1f;
	_contactData._tolerance = 0.1f;

	LoadGround();
}

void Level6::Update(float dt)
{
	//Applies gravity to our rigid bodies
	_pGravityGenerator->Update(_pCubeRb.get(), dt);
	_pGravityGenerator->Update(_pSphereRb.get(), dt);

	//Calculates our colliders new transforms
	_pSphereCollider->CalculateInternals();
	_pBoxCollider->CalculateInternals();

	//Resets the amount of contacts in the scene
	_contactData.Reset(LEVEL_6_MAX_CONTACTS);

	//Detect collisions between all types of objects
	CollisionDetector::BoxAndSphere(*_pBoxCollider, *_pSphereCollider, &_contactData);
	CollisionDetector::SphereAndTruePlane(*_pSphereCollider, *_pGroundCollider, &_contactData);
	CollisionDetector::BoxAndPlane(*_pBoxCollider, *_pGroundCollider, &_contactData);

	//Resolve any contacts this frame
	_pContactResolver->ResolveContacts(_contactData._contactArray, _contactData._contactCount, dt);
}




void Level6::DrawUI()
{
	ImGui::Begin("Test 6");

	ImGui::Text("This test showcases two Rigidbodies colliding with each other, however unlike the previous tests");
	ImGui::Text("This test involves the collision detection for spheres as well. This is to show the addition of multiple collider types");
	ImGui::Text("The objects are forced downwards through gravity and will then collide and react");

	ImGui::Text("Contacts this frame: %d", _contactData._contactCount);

	ResetButton();

	ImGui::End();
}
