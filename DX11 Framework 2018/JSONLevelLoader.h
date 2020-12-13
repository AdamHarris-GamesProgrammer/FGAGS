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
	JSONLevelLoader() = default;
	JSONLevelLoader(Graphics* gfx);


	//Loads the objects from the file
	std::vector<GameObject*> LoadObjectsFromFile(const char* filename);

private:
	Graphics* pGfx;

	wchar_t* ConvertString(std::string& str);

};

