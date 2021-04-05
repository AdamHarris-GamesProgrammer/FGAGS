#include "Level6.h"

void Level6::ExitLevel()
{

}

void Level6::LoadLevel()
{
	Level::LoadLevel();

	_pGravityGenerator = new GravityForceGenerator(Vector3(0.0f, -9.81f, 0.0f));

	_pSphereRb = new RigidbodyComponent(_pGameObjects[0]);
	_pSphereRb->SetAwake();
	_pSphereRb->SetMass(20.0f);
	_pSphereCollider = new Sphere(_pSphereRb);

	_pCubeRb = new RigidbodyComponent(_pGameObjects[1]);
	_pCubeRb->SetAwake();
	_pBoxCollider = new Box(_pCubeRb, Vector3(1.0, 1.0, 1.0));
	

	_pGroundCollider = new CollisionPlane(Vector3(0, 1, 0), 0.0f);

	_pContactResolver = new ContactResolver(MAX_CONTACTS);
	_contactData._contactArray = _contactsArray;
	_contactData._friction = 0.9;
	_contactData._restitution = 0.1;
	_contactData._tolerance = 0.1;

	LoadGround();
}

void Level6::Update(float dt)
{
	_pGravityGenerator->Update(_pCubeRb, dt);
	_pGravityGenerator->Update(_pSphereRb, dt);

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
}




void Level6::DrawUI()
{
	ImGui::Begin("Test 6");

	ImGui::Text("Contacts this frame: %d", _contactData._contactCount);

	ResetButton();

	ImGui::End();
}
