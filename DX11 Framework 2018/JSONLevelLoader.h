#pragma once
#include "Graphics.h"
#include "MeshedObject.h"
#include <nlohmann/json.hpp>
#include <string>
#include <fstream>
#include <vector>

class JSONLevelLoader
{
public:
	JSONLevelLoader(Graphics* gfx);

	std::vector<GameObject*> LoadObjectsFromFile(const char* filename);

private:
	Graphics* mGraphics;

};

