#include "Level3.h"

void Level3::LoadLevel()
{
	Level::LoadLevel();

	if (_pRb == nullptr) {
		_pRb = std::make_unique<RigidbodyComponent>(_pGameObjects[0]);
		_pRb->SetAngularDamping(0.9);
		_pRb->SetLinearDamping(1.0);
		_pRb->SetAwake();
		_pRb->SetCanSleep(false);
		_pRb->SetCubeInertiaTensor();
		_pRb->SetInverseMass(5.0f);
	}
	else
	{
		_pRb->SetOwner(_pGameObjects[0]);
	}

	_pRb->ClearAccumulator();

	_rotationPower = 100.0f;

	_normalizedCoords[0] = 0.0f;
	_normalizedCoords[1] = 0.0f;
}

void Level3::ExitLevel()
{
}

void Level3::Update(float dt)
{
	//Stops object from falling down
	_pRb->SetVelocity(Vector3());


	if (_pGfx->IsLeftMouseDown()) {
		//Get the mouse positions
		int mouseX = _pGfx->GetMouseX();
		int mouseY = _pGfx->GetMouseY();

		//Convert it into NDC
		_normalizedCoords[0] = (2.0f * mouseX) / _pGfx->GetWindowWidth() - 1.0f; //Mouse X Rotation Y
		_normalizedCoords[1] = 1.0f - (2.0f * mouseY) / _pGfx->GetWindowHeight(); //Mouse Y Rotation X


		//Variable to scale our rotation by
		float rotationXScale = 1.0f;
		float rotationYScale = 1.0f;

		//multiply the scale by the NDC for each axis
		rotationXScale *= _normalizedCoords[0];
		rotationYScale *= _normalizedCoords[1];

		//Get the absolute value to scale by
		rotationXScale = fabsf(rotationXScale);
		rotationYScale = fabsf(rotationYScale);

		//Calculate force to add to object 
		//using mouse Y for rotating on X axis and mouse X for rotating on Y axis. 
		//Inverting the mouse Y NDC as DirectX goes from -1 
		_pRb->AddTorque(Vector3(-_normalizedCoords[1] * rotationYScale, _normalizedCoords[0] * rotationXScale, 0) * _rotationPower);
	}
}

void Level3::DrawUI()
{
	ImGui::Begin("Test 3");

	ImGui::Text("NDC: %.2f %.2f", _normalizedCoords[0], _normalizedCoords[1]);
	ImGui::SliderFloat("Rotation Power: ", &_rotationPower, 0.0f, 100.0f, "%.2f", 1.0f);

	ResetButton();

	ImGui::End();
}
