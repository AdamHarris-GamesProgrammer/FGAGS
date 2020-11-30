#include "Application.h"
#include <iostream>
#include <DirectXMath.h>

#include "Imgui/imgui.h"



Application::Application() {}

Application::~Application()
{
	delete cameraA;

	cameraA = nullptr;
}

HRESULT Application::Initialise(HINSTANCE hInstance, int nCmdShow)
{
	graphics = new Graphics();
	graphics->Initialise(hInstance, nCmdShow);

	groundPlane = new Plane(graphics);
	groundPlane->Make(75.0, 75.0f, 8, 8);


	mImGuiManager = new ImGUIManager(graphics);


	mJSONLevelLoader = new JSONLevelLoader(graphics);

	mGameObjects = mJSONLevelLoader->LoadObjectsFromFile("Assets/Levels/level.json");

	mGameObjects.push_back(groundPlane);

	cube = FindGameObjectWithName("Cube");
	donut = FindGameObjectWithName("Donut");

	groundPlane->CreateTexture(L"Assets/Textures/stone.dds");


	cameraA = new Camera();
	cameraA->SetLens(0.25f * 3.1452, graphics->GetWindowWidth() / graphics->GetWindowHeight(), 0.01f, 100.0f);
	
	mCurrentCamera = cameraA;
	graphics->SwitchCamera(cameraA);

	cameraB = new Camera();

	cameraA->LookAt(
		XMFLOAT3(0.0f, 8.0f, -15.0f),
		XMFLOAT3(0.0f, 0.0f, 0.0f)
	);

	cameraA->UpdateViewMatrix();

	//Sets default positions
	cube->SetPosition(-8.0f, 0.0f, 0.0f);
	groundPlane->SetPosition(0.0f, -1.6f, 0.0f);

	rotationValue = 0.0f;

	time = Time();

	time.Reset();

	return S_OK;
}

void Application::Update()
{
	time.Tick();
	float dt = time.DeltaTime();

	WireframeControls(dt);
	CursorControls(dt);

	if (enableFlying)
	{
		CameraControls(dt);
	}

	if (GetAsyncKeyState('1')) {
		mCurrentCamera = cameraA;
		graphics->SwitchCamera(cameraA);
	}
	else if (GetAsyncKeyState('2')) {
		mCurrentCamera = cameraB;
		graphics->SwitchCamera(cameraB);
	}

	cameraB->SetPosition(cube->GetPosition());

	XMFLOAT3 cubePos = cube->GetPosition();
	XMFLOAT3 newCameraBPos = XMFLOAT3
	(
		cubePos.x + cameraBOffset.x,
		cubePos.y + cameraBOffset.y,
		cubePos.z + cameraBOffset.z
	);


	float dx = XMConvertToRadians(0.25f * static_cast<float>(graphics->GetMouseX() - mLastMousePosX));
	cameraB->RotateY(dx);

	cameraB->LookAt
	(
		newCameraBPos,
		cubePos
	);

	rotationValue += (rotationSpeed * dt);

	cube->SetRotation(10.0f, rotationValue, 0.0f);
	donut->SetRotation(rotationValue, 0.0f, 0.0f);

	mCurrentCamera->UpdateViewMatrix();

	for (auto& object : mGameObjects) {
		object->Update(dt);
	}
}

void Application::Picking()
{
	int mouseX = graphics->GetMouseX();
	int mouseY = graphics->GetMouseY();

	XMMATRIX invView = XMMatrixInverse(nullptr, mCurrentCamera->View());
	XMMATRIX invProj = XMMatrixInverse(nullptr, mCurrentCamera->Proj());

	float normalizedCoords[2];
	//Near Window Width and Height may not be the correct thing
	normalizedCoords[0] = (2.0f * mouseX) / mCurrentCamera->GetNearWindowWidth() - 1.0f;
	normalizedCoords[1] = 1.0f - (2.0f * mouseY) / mCurrentCamera->GetNearWindowHeight();

	XMVECTOR eyePos;
	XMVECTOR dummy;
	XMMatrixDecompose(&dummy, &dummy, &eyePos, invView);

	XMVECTOR rayOrigin = XMVectorSet(normalizedCoords[0], normalizedCoords[1], 0, 0);
	rayOrigin = XMVector3Transform(rayOrigin, invProj);


	rayOrigin = XMVector3Transform(rayOrigin, invView);

	XMVECTOR rayDirection = rayOrigin - eyePos;

	rayDirection = XMVector2Normalize(rayDirection);



}

void Application::WireframeControls(float dt)
{
	bool changed = false;
	timeSinceSpacePressed += dt;
	if (GetAsyncKeyState(VK_SPACE)) {
		if (timeSinceSpacePressed > spaceTimer) {
			wireframeOn = !wireframeOn;
			changed = true;
			timeSinceSpacePressed = 0.0f;
		}
	}
	if (changed) graphics->EnableWireframe(wireframeOn);
}

void Application::CursorControls(float dt)
{
	timeSinceFPressed += dt;
	if (GetAsyncKeyState('F')) {
		if (timeSinceFPressed > fTimer) {
			timeSinceFPressed = 0.0f;

			enableFlying = !enableFlying;

			clippedCursor = !clippedCursor;
			if (clippedCursor) {
				graphics->HideCursor();
				ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
			}
			else
			{
				graphics->ShowCursor();
				ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
			}
		}
	}
}

void Application::CameraControls(float dt)
{
	if (GetAsyncKeyState('W')) cameraA->Walk(10.0f * dt);
	if (GetAsyncKeyState('S')) cameraA->Walk(-10.0f * dt);
	if (GetAsyncKeyState('A')) cameraA->Strafe(-10.0f * dt);
	if (GetAsyncKeyState('D')) cameraA->Strafe(10.0f * dt);

	float dx = XMConvertToRadians(0.25f * static_cast<float>(graphics->GetMouseX() - mLastMousePosX));
	float dy = XMConvertToRadians(0.25f * static_cast<float>(graphics->GetMouseY() - mLastMousePosY));

	mCurrentCamera->Pitch(dy);
	mCurrentCamera->RotateY(dx);

	mLastMousePosX = graphics->GetMouseX();
	mLastMousePosY = graphics->GetMouseY();
}

void Application::Draw()
{
	mImGuiManager->BeginFrame();

	graphics->ClearBuffers();

	for (auto& object : mGameObjects) {
		object->Draw();
	}

	mImGuiManager->EndFrame();

	graphics->Present();

}

GameObject* Application::FindGameObjectWithName(std::string name)
{
	for (auto& obj : mGameObjects) {
		if (obj->GetName() == name) return obj;
	}
}
