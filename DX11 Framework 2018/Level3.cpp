#include "Level3.h"

void Level3::LoadLevel()
{
	Level::LoadLevel();

	_pGameObjects[0]->GetBody()->SetAngularDamping(0.9);
	_pGameObjects[0]->GetBody()->SetLinearDamping(1.0);
	_pGameObjects[0]->GetBody()->SetInverseMass(0.0);
	_pGameObjects[0]->GetBody()->SetAwake();
	_pGameObjects[0]->GetBody()->SetCanSleep(false);
}

void Level3::ExitLevel()
{
}

void Level3::PollInput(float dt)
{
	Level::PollInput(dt);
}

void Level3::Update(float dt)
{
	_pGameObjects[0]->GetBody()->SetVelocity(Vector3());


	if (GetAsyncKeyState('Y')) {
		//Get the mouse positions
		int mouseX = _pGfx->GetMouseX();
		int mouseY = _pGfx->GetMouseY();

		//Convert it into NDC
		float normalizedCoords[2];
		normalizedCoords[0] = (2.0f * mouseX) / _pGfx->GetWindowWidth() - 1.0f;
		normalizedCoords[1] = 1.0f - (2.0f * mouseY) / _pGfx->GetWindowHeight();

		//Calculate force to add to object 
		_pGameObjects[0]->GetBody()->AddTorque(Vector3(10, 0, 0));
	}
}

void Level3::Reset()
{
	LoadLevel();

}

void Level3::DrawUI()
{
	ImGui::Begin("Test 3");

	ImGui::Text("Test 3");

	ResetButton();

	ImGui::End();
}
