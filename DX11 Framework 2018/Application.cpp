#include "Application.h"
#include <iostream>
#include <DirectXMath.h>

#include "Imgui/imgui.h"

#include <thread>
#include <string>

Application::~Application()
{
	delete _pGfx;
	_pGfx = nullptr;
}

HRESULT Application::Initialise(HINSTANCE hInstance, int nCmdShow)
{
	//Initializes the Graphics object
	_pGfx = new Graphics();
	_pGfx->Initialise(hInstance, nCmdShow);

	//Sky blue colour
	_pGfx->SetClearColor(new float[4]{ 0.583f, 0.639f, 0.743f, 1.0f });


	//Initializes and resets the Timer object
	_time = Time();
	_time.Reset();

	//Initialize all of our levels 
	_pLevel0 = std::make_shared<Level0>(_pGfx, "Assets/Levels/test0.json");
	_pLevel1 = std::make_shared<Level1>(_pGfx, "Assets/Levels/test1.json");
	_pLevel2 = std::make_shared<Level2>(_pGfx, "Assets/Levels/test2.json");
	_pLevel3 = std::make_shared<Level3>(_pGfx, "Assets/Levels/test3.json");
	_pLevel4 = std::make_shared<Level4>(_pGfx, "Assets/Levels/test4.json");
	_pLevel5 = std::make_shared<Level5>(_pGfx, "Assets/Levels/test5.json");
	_pLevel6 = std::make_shared<Level6>(_pGfx, "Assets/Levels/test6.json");
	_pLevel7 = std::make_shared<Level7>(_pGfx, "Assets/Levels/test7.json");

	//Switch t0 level 0 (home screen)
	ChangeLevel(_pLevel0);

	return S_OK;
}

void Application::Update()
{
	//Ticks the Timer object and gets the new delta time value
	_time.Tick();
	float dt = _time.DeltaTime();

	//Polls the input, in this case it is checking if we want to switch levels
	PollInput(dt);

	//Polls input for the current level
	_pCurrentLevel->PollInput(dt);

	//Begins the update for the current level, this method updates the objects and their components
	_pCurrentLevel->BeginUpdate(dt);

	//Updates the current level, this is where any custom logic will take place
	_pCurrentLevel->Update(dt);
}

void Application::PollInput(float dt)
{
	//This method checks if any of the keys 0 to 7 are done and changes to the required level if we want to
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

void Application::ChangeLevel(std::shared_ptr<Scene> newLevel)
{
	//if we have a current level exit it
	if (_pCurrentLevel != nullptr) _pCurrentLevel->ExitLevel();

	//load the new level
	_pCurrentLevel = newLevel;

	//reset the new level
	_pCurrentLevel->Reset();
}

void Application::Draw()
{
	//Draws the current level
	_pCurrentLevel->Render();
}
