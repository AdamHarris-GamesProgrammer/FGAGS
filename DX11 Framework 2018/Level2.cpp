#include "Level2.h"

void Level2::LoadLevel()
{
	Level::LoadLevel();

	//LoadGround();

	_pRb = new RigidbodyComponent(_pGameObjects[0]);
	_pRb->SetAwake();

	_pGravityGenerator = new GravityForceGenerator(Vector3(0.0f, -9.81f, 0.0f));
	_pRestingGenerator = new RestingForceGenerator(1.0f);
}

void Level2::ExitLevel()
{

}

void Level2::Update(float dt)
{
	_pGravityGenerator->Update(_pRb, dt);
	_pRestingGenerator->Update(_pRb, dt);
}

void Level2::DrawUI()
{
	ImGui::Begin("Test 2");

	OutputVector3("Object Velocity: ", _pRb->GetVelocity());

	ResetButton();

	ImGui::End();
}
