#include "Level2.h"

void Level2::LoadLevel()
{
	Scene::LoadLevel();

	LoadGround();

	//Creates the rigidbody object
	if (_pRb == nullptr) _pRb = std::make_unique<RigidbodyComponent>(_pGameObjects[0]);
	else _pRb->SetOwner(_pGameObjects[0]);

	//Creates the force generator objects
	if(_pGravityGenerator == nullptr) _pGravityGenerator = std::make_unique<GravityForceGenerator>(Vector3(0.0f, -9.81f, 0.0f));
	if(_pRestingGenerator == nullptr) _pRestingGenerator = std::make_unique<RestingForceGenerator>(1.0f);
}

void Level2::Update(float dt)
{
	_pGravityGenerator->Update(_pRb.get(), dt);
	_pRestingGenerator->Update(_pRb.get(), dt);
}

void Level2::DrawUI()
{
	ImGui::Begin("Test 2");

	ImGui::Text("This test showcases a gravity and resting force generator being applied to a rigidbody.");
	ImGui::Text("This is to make a cube fall, hit the floor and then stop falling.");

	OutputVector3("Object Position: ", _pGameObjects[0]->GetTransform().GetPosition());
	OutputVector3("Object Velocity: ", _pRb->GetVelocity());

	ResetButton();

	ImGui::End();
}
