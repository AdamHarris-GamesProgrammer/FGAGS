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

	delete pGfx;
	pGfx = nullptr;
}

HRESULT Application::Initialise(HINSTANCE hInstance, int nCmdShow)
{
	//Initializes the Graphics object
	pGfx = new Graphics();
	pGfx->Initialise(hInstance, nCmdShow);


	//Initializes and resets the Timer object
	mTime = Time();
	mTime.Reset();

	pGfx->SetClearColor(mClearColor);

	_pLevel1 = new Level1(pGfx, "Assets/Levels/test1.json");
	_pLevel2 = new Level2(pGfx, "Assets/Levels/test2.json");
	_pLevel3 = new Level3(pGfx, "Assets/Levels/test3.json");
	_pLevel4 = new Level4(pGfx, "Assets/Levels/test4.json");
	_pLevel5 = new Level5(pGfx, "Assets/Levels/test5.json");


	ChangeLevel(_pLevel1);

	return S_OK;
}

void Application::Update()
{
	//Ticks the Timer object and gets the new delta time value
	mTime.Tick();
	float dt = mTime.DeltaTime();

	PollInput(dt);

	_pCurrentLevel->PollInput(dt);

	_pCurrentLevel->BeginUpdate(dt);

	_pCurrentLevel->Update(dt);

	_pCurrentLevel->EndUpdate(dt);
}

void Application::DrawGUI()
{
	//Simulation Settings Window
	{
		bool originalWireframe = mWireframeEnabled;

		bool* open = new bool(true);

		//Opens a Window called "Simulation Settings, which defaults to open and is non re sizable
		ImGui::Begin("Simulation Settings", open, ImGuiWindowFlags_NoResize);

		//C3
		//Wireframe Checkbox
		ImGui::Text("Wireframe Mode: ");
		ImGui::SameLine();
		ImGui::Checkbox("###", &mWireframeEnabled);
		if (originalWireframe != mWireframeEnabled) pGfx->SetWireframe(mWireframeEnabled);

		//Background Clear Color 
		ImGui::Text("Background Clear Colour: ");
		ImGui::NewLine();
		ImGui::PushItemWidth(100.0f);
		ImGui::ColorPicker4("####", mClearColor);
		pGfx->SetClearColor(mClearColor);

		ImGui::End();
	}

	//Controls Window
	{
		ImGui::Begin("Controls");

		//Camera A Controls
		ImGui::Text("Camera A (Debug Fly Cam): 1");
		ImGui::Text("Camera A Enable Movement: G");
		if (ImGui::TreeNode("Camera A Movement Controls")) {
			ImGui::Text("Walk Forwards: W");
			ImGui::Text("Walk Backwards: S");
			ImGui::Text("Walk Left: A");
			ImGui::Text("Walk Right: D");
			ImGui::Text("Rotate Left: Q");
			ImGui::Text("Rotate Right: E");
			ImGui::Text("Pitch Up: R");
			ImGui::Text("Pitch Down: F");
			ImGui::TreePop();
		}
		ImGui::Text("Camera B (Third Person Cam): 2");
		ImGui::Text("Camera C (Top Down Cam): 3");
		ImGui::Text("Camera D (Level Overview): 4");

		//Selected Object Controls
		ImGui::Text("Select Object with Mouse Position: H");
		if (ImGui::TreeNode("Selected Object Controls")) {
			ImGui::Text("Move Forwards: W");
			ImGui::Text("Move Backwards: S");
			ImGui::Text("Move Left: A");
			ImGui::Text("Move Right: D");
			ImGui::Text("Press Y to deselect current object");
			ImGui::TreePop();
		}


		ImGui::End();
	}

	//Selected Object Window

	//Calls the lighting control panel method in the graphics class
	pGfx->LightingWindow();
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
	CursorControls(dt);

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


	if (GetAsyncKeyState('U')) {
		__debugbreak();
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

void Application::CursorControls(float dt)
{
	mTimeSinceGPressed += dt;
	if (GetAsyncKeyState('G')) {
		if (mTimeSinceGPressed > mGKeyTimer) {
			mTimeSinceGPressed = 0.0f;

			mFlyingEnabled = !mFlyingEnabled;

			mCursorClipped = !mCursorClipped;
			if (mCursorClipped) {
				pGfx->HideCursor();
				ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
			}
			else
			{
				pGfx->ShowCursor();
				ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
			}
		}
	}
}

void Application::Draw()
{
	_pCurrentLevel->Render();
}
