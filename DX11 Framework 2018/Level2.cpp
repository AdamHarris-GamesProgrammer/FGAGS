#include "Level2.h"

void Level2::LoadLevel()
{
	Level::LoadLevel();

	LoadGround();

	_pRb = new RigidbodyComponent(_pGameObjects[0]);
	_pRb->SetAwake();
	_pRb->SetMass(10.0f);

	if(_pGravityGenerator == nullptr) _pGravityGenerator = std::make_unique<GravityForceGenerator>(Vector3(0.0f, -9.81f, 0.0f));
	if(_pRestingGenerator == nullptr) _pRestingGenerator = std::make_unique<RestingForceGenerator>(1.0f);
}

void Level2::ExitLevel()
{
	delete _pRb;
	_pRb = nullptr;

	_pGameObjects.clear();

}

void Level2::Update(float dt)
{
	_pGravityGenerator->Update(_pRb, dt);
	_pRestingGenerator->Update(_pRb, dt);
}

void Level2::DrawUI()
{
	ImGui::Begin("Test 2");

	OutputVector3("Object Position: ", _pGameObjects[0]->GetTransform().GetPosition());
	OutputVector3("Object Velocity: ", _pRb->GetVelocity());

	ResetButton();

	ImGui::End();
}
