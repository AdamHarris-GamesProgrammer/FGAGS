#include "Application.h"
#include <iostream>
#include <DirectXMath.h>

#include "Imgui/imgui.h"

#include <thread>
#include <string>

Application::~Application()
{
	delete _pLevel1;
	_pLevel1 = nullptr;

	delete _pLevel2;
	_pLevel2 = nullptr;

	delete _pLevel3;
	_pLevel3 = nullptr;

	delete _pLevel4;
	_pLevel4 = nullptr;

	delete _pLevel5;
	_pLevel5 = nullptr;

	delete _pLevel6;
	_pLevel6 = nullptr;

	delete _pLevel7;
	_pLevel7 = nullptr;

	delete _pGfx;
	_pGfx = nullptr;
}

HRESULT Application::Initialise(HINSTANCE hInstance, int nCmdShow)
{
	//Initializes the Graphics object
	_pGfx = new Graphics();
	_pGfx->Initialise(hInstance, nCmdShow);


	//Initializes and resets the Timer object
	_time = Time();
	_time.Reset();

	_pGfx->SetClearColor(_clearColor);

	_pLevel1 = new Level1(_pGfx, "Assets/Levels/test1.json");
	_pLevel2 = new Level2(_pGfx, "Assets/Levels/test2.json");
	_pLevel3 = new Level3(_pGfx, "Assets/Levels/test3.json");
	_pLevel4 = new Level4(_pGfx, "Assets/Levels/test4.json");
	_pLevel5 = new Level5(_pGfx, "Assets/Levels/test5.json");
	_pLevel6 = new Level6(_pGfx, "Assets/Levels/test6.json");
	_pLevel7 = new Level7(_pGfx, "Assets/Levels/test7.json");

	ChangeLevel(_pLevel7);

	return S_OK;
}

void Application::Update()
{
	//Ticks the Timer object and gets the new delta time value
	_time.Tick();
	float dt = _time.DeltaTime();

	PollInput(dt);

	_pCurrentLevel->PollInput(dt);

	_pCurrentLevel->BeginUpdate(dt);
	_pCurrentLevel->Update(dt);
}

void Application::Picking()
{
	//int mouseX = pGfx->GetMouseX();
	//int mouseY = pGfx->GetMouseY();

	//XMMATRIX invView = XMMatrixInverse(nullptr, pCurrentCamera->View());
	//XMMATRIX invProj = XMMatrixInverse(nullptr, pCurrentCamera->Proj());

	////Convert mouse position to NDC (Normalized Device Coordinates)
	//float normalizedCoords[2];
	//normalizedCoords[0] = (2.0f * mouseX) / pGfx->GetWindowWidth() - 1.0f;
	//normalizedCoords[1] = 1.0f - (2.0f * mouseY) / pGfx->GetWindowHeight();

	////Sets the X and Y positions for the origin of the ray
	//XMVECTOR rayOrigin = XMVectorSet(normalizedCoords[0], normalizedCoords[1], 0, 0);

	////Converts it from screen space to projection space
	//rayOrigin = XMVector3Transform(rayOrigin, invProj);

	////Converts from screen space to view space
	//rayOrigin = XMVector3Transform(rayOrigin, invView);

	////Calculates the direction of the ray
	//XMVECTOR rayDirection = rayOrigin - XMLoadFloat3(&(XMFLOAT3)pCurrentCamera->GetTransform().GetPosition());

	////Normalizes the direction
	//rayDirection = XMVector3Normalize(rayDirection);

	////Stores them as a XMFLOAT4 to allow passing between functions
	//XMFLOAT4 origin;
	//XMFLOAT4 direction;
	//XMStoreFloat4(&origin, rayOrigin);
	//XMStoreFloat4(&direction, rayDirection);

	//bool objectFound = false;

	////Loops through all objects in the scene 
	//for (auto& object : pGameObjects) {
	//	//Tests collision with each object
	//	if (object->TestCollision(origin, direction)) {
	//		//Sets the selected object
	//		pSelectedObject = object;

	//		objectFound = true;

	//		//Breaks when a object has been selected, stops it from selecting objects behind the original object
	//		break;
	//	}
	//}

	////Allows user to deselect an item
	//if (!objectFound) pSelectedObject = nullptr;
}

void Application::PollInput(float dt)
{
	if (GetAsyncKeyState('1')) {
		ChangeLevel(_pLevel1);
	}
	else if (GetAsyncKeyState('2')) {
		ChangeLevel(_pLevel2);
	}
	else if (GetAsyncKeyState('3')) {
		ChangeLevel(_pLevel3);
	}
	else if (GetAsyncKeyState('4')) {
		ChangeLevel(_pLevel4);
	}
	else if (GetAsyncKeyState('5')) {
		ChangeLevel(_pLevel5);
	}
	else if (GetAsyncKeyState('6')) {
		ChangeLevel(_pLevel6);
	}
	else if (GetAsyncKeyState('7')) {
		ChangeLevel(_pLevel7);
	}
}

void Application::ChangeLevel(Level* newLevel)
{
	//if we have a current level exit it
	if (_pCurrentLevel != nullptr)_pCurrentLevel->ExitLevel();

	//load the new level
	_pCurrentLevel = newLevel;

	//reset the new level
	_pCurrentLevel->Reset();
}

void Application::Draw()
{
	_pCurrentLevel->Render();
}
