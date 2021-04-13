#pragma once
#include "Graphics.h"
#include "Object.h"
#include <nlohmann/json.hpp>
#include <string>
#include <fstream>
#include <vector>

using json = nlohmann::json;

class JSONLevelLoader
{
public:
	JSONLevelLoader() = default;

	//Loads the objects from the file
	std::vector<Object*> LoadObjectsFromFile(const char* filename, Graphics* pGfx);
	std::vector<std::shared_ptr<Camera>> LoadCamerasFromFile(const char* filename);
private:
	wchar_t* ConvertString(std::string& str);
	json LoadJSONFile(const char* filename);
	bool HasAttribute(json* file, std::string key);

};

