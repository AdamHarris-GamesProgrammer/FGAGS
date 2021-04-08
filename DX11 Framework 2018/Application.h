#pragma once

#include <windows.h>
#include <memory>

#include "Graphics.h"
#include "Time.h"

#include "Scene.h"
#include "Level0.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Level4.h"
#include "Level5.h"
#include "Level6.h"
#include "Level7.h"

class Application
{
public:
	Application() = default;
	~Application();

	//Initializes all the data within the engine and initializes all of DirectX
	HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);

	//Performs per-frame logic
	void Update();

private:
	//Checks for any input that the engine can deal with
	void PollInput(float dt);

	//Changes our current level
	void ChangeLevel(std::shared_ptr<Scene> newLevel);

private:
	//Stores the current level we are in so we only draw and update this scene
	std::shared_ptr<Scene> _pCurrentLevel;

	//Stores all of our levels we use
	//Using smart pointers here as they automatically deallocate themselves when needed
	std::shared_ptr<Level0> _pLevel0 = nullptr;
	std::shared_ptr<Level1> _pLevel1 = nullptr;
	std::shared_ptr<Level2> _pLevel2 = nullptr;
	std::shared_ptr<Level3> _pLevel3 = nullptr;
	std::shared_ptr<Level4> _pLevel4 = nullptr;
	std::shared_ptr<Level5> _pLevel5 = nullptr;
	std::shared_ptr<Level6> _pLevel6 = nullptr;
	std::shared_ptr<Level7> _pLevel7 = nullptr;
	

	//Time object used to track delta time for physics simulations
	Time _time;

	//Stores a pointer to the graphics class which is used by all objects
	Graphics* _pGfx = nullptr;
};

