#include "JSONLevelLoader.h"

using json = nlohmann::json;

JSONLevelLoader::JSONLevelLoader(Graphics* gfx)
	: mGraphics(gfx)
{

}

std::vector<GameObject*> JSONLevelLoader::LoadObjectsFromFile(const char* filename)
{



	std::vector<GameObject*> gameObjects;

	if (mGraphics == nullptr) return gameObjects;

	std::ifstream inFile("Assets/Levels/level.json");
	json jsonFile;

	if (!inFile.is_open()) {
		__debugbreak();
	}

	inFile >> jsonFile;

	json gameobjects = jsonFile["gameobjects"];
	int size = gameobjects.size();


	auto HasAttribute = [](json* file, std::string key) {
		if (file->contains(key)) return true;

		return false;
	};

	for (unsigned int i = 0; i < size; i++) {
		bool hasDifTexture = false;
		bool hasSpcTexture = false;
		bool hasNrmTexture = false;

		MeshedObject* go = new MeshedObject(mGraphics);
		json jsonGo = gameobjects.at(i);
		std::string name = jsonGo["name"];
		go->SetName(name);

		if (HasAttribute(&jsonGo, "position")) {
			std::vector<float> position = jsonGo["position"];
			go->SetPosition(position[0], position[1], position[2]);
		}

		if (HasAttribute(&jsonGo, "rotation")) {
			std::vector<float> rotation = jsonGo["rotation"];
			go->SetRotation(rotation[0], rotation[1], rotation[2]);
		}

		if (HasAttribute(&jsonGo, "scale")) {
			std::vector<float> scale = jsonGo["scale"];
			go->SetScale(scale[0], scale[1], scale[2]);
		}

		if (HasAttribute(&jsonGo, "meshPath")) {

			std::string meshName = jsonGo["meshPath"];
			std::string meshPath = "Assets/Models/" + meshName;
			go->Load(meshPath);
		}

		if (HasAttribute(&jsonGo, "diffuseTexture")) {
			hasDifTexture = true;

			std::string textureName = jsonGo["diffuseTexture"];
			std::string path = "Assets/Textures/" + textureName;

			wchar_t* convertedPath = new wchar_t[path.length() + 1];
			mbstowcs_s(0, convertedPath, path.length() + 1, path.data(), _TRUNCATE);

			go->CreateTexture(convertedPath);
		}

		if (HasAttribute(&jsonGo, "specularTexture")) {
			hasSpcTexture = true;

			std::string textureName = jsonGo["specularTexture"];
			std::string path = "Assets/Textures/" + textureName;

			wchar_t* convertedPath = new wchar_t[path.length() + 1];
			mbstowcs_s(0, convertedPath, path.length() + 1, path.data(), _TRUNCATE);

			go->CreateTexture(convertedPath);
		}

		if (HasAttribute(&jsonGo, "normalTexture")) {
			hasNrmTexture = true;

			std::string textureName = jsonGo["normalTexture"];
			std::string path = "Assets/Textures/" + textureName;

			wchar_t* convertedPath = new wchar_t[path.length() + 1];
			mbstowcs_s(0, convertedPath, path.length() + 1, path.data(), _TRUNCATE);

			go->CreateTexture(convertedPath);
		}

		if (hasNrmTexture) {
			go->PhongDifSpcNrmShader();
		}
		else if (hasSpcTexture) {
			go->PhongDifSpcShader();
		}
		else if (hasDifTexture) {
			go->PhongDifShader();
		}

		gameObjects.push_back(go);
	}

	return gameObjects;
}
