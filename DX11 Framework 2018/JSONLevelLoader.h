#pragma once
#include "Graphics.h"
#include "MeshedObject.h"
#include <nlohmann/json.hpp>
#include <string>
#include <fstream>
#include <vector>

using json = nlohmann::json;

class JSONLevelLoader
{
public:
	JSONLevelLoader() = default;
	JSONLevelLoader(Graphics* gfx);


	//Loads the objects from the file
	std::vector<GameObject*> LoadObjectsFromFile(const char* filename);
	std::vector<std::shared_ptr<Camera>> LoadCamerasFromFile(const char* filename);

	

private:
	Graphics* pGfx;

	wchar_t* ConvertString(std::string& str);
	json LoadJSONFile(const char* filename);
	bool HasAttribute(json* file, std::string key);

};

