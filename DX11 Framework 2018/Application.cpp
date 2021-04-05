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

	_pLevel0 = new Level0(_pGfx, "Assets/Levels/test0.json");
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

void Application::PollInput(float dt)
{
	if (GetAsyncKeyState('0')) {
		ChangeLevel(_pLevel0);
	}
	else if (GetAsyncKeyState('1')) {
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
