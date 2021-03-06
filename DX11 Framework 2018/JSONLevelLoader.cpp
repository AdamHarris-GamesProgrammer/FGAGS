#include "JSONLevelLoader.h"
#include "RendererComponent.h"
#include <memory>

std::vector<Object*> JSONLevelLoader::LoadObjectsFromFile(const char* filename, std::shared_ptr<Graphics> pGfx)
{
	//Base Gameobject vector
	std::vector<Object*> objects;

	//Checks the pGfx object has been set
	if (pGfx == nullptr) return objects;

	//Loads the contents of the file into a JSON object
	json jsonFile = LoadJSONFile(filename);

	//Sets a separate json object for the gameobjects collection  from the file
	json gameobjects = jsonFile["meshedObjects"];
	//Gets the size
	int size = gameobjects.size();

	//Sets the capacity for the vector
	objects.reserve(size);
	for (int i = 0; i < size; i++) {
		//These bool values are used to see which shader should be used
		bool hasDifTexture = false;
		bool hasSpcTexture = false;
		bool hasNrmTexture = false;

		//Creates a meshed object 
		Object* go = new Object();

		RendererComponent* renderer = new RendererComponent(go, pGfx);

		//Gets the current game object data
		json jsonGo = gameobjects.at(i);

		//Checks for the name attribute and sets the objects name
		if (HasAttribute(&jsonGo, "name")) {
			std::string name = jsonGo["name"];
			go->SetName(name);
		}

		//Checks for the position attribute and sets the objects position
		if (HasAttribute(&jsonGo, "position")) {
			std::vector<float> position = jsonGo["position"];
			go->GetTransform().SetPosition(position[0], position[1], position[2]);
		}

		//Checks for the rotation attribute and sets the objects rotation
		if (HasAttribute(&jsonGo, "rotation")) {
			std::vector<float> rotation = jsonGo["rotation"];
			go->GetTransform().SetRotation(rotation[0], rotation[1], rotation[2]);
		}

		//Checks for the scale attribute and sets the objects scale
		if (HasAttribute(&jsonGo, "scale")) {
			std::vector<float> scale = jsonGo["scale"];
			go->GetTransform().SetScale(scale[0], scale[1], scale[2]);
		}

		//Checks for the mesh path attribute and loads the mesh
		if (HasAttribute(&jsonGo, "meshPath")) {

			std::string meshName = jsonGo["meshPath"];
			std::string meshPath = "Assets/Models/" + meshName;
			renderer->Load(meshPath);
		}

		//Checks for the diffuse texture attribute and loads the texture
		if (HasAttribute(&jsonGo, "diffuseTexture")) {
			hasDifTexture = true;

			//Gets the textures name
			std::string textureName = jsonGo["diffuseTexture"];

			//Loads the desired texture
			wchar_t* texturePath = ConvertString("Assets/Textures/" + textureName);
			renderer->CreateTexture(texturePath);
		}

		//Checks for the specular texture attribute and loads the texture
		if (HasAttribute(&jsonGo, "specularTexture")) {
			hasSpcTexture = true;
			std::string textureName = jsonGo["specularTexture"];
			wchar_t* texturePath = ConvertString("Assets/Textures/" + textureName);
			renderer->CreateTexture(texturePath);
		}

		//Checks for the normal texture attribute and loads the texture 
		if (HasAttribute(&jsonGo, "normalTexture")) {
			hasNrmTexture = true;
			std::string textureName = jsonGo["normalTexture"];
			wchar_t* texturePath = ConvertString("Assets/Textures/" + textureName);
			renderer->CreateTexture(texturePath);
		}

		//Checks for the ambient attribute and sets the objects ambient material
		if (HasAttribute(&jsonGo, "ambient")) {
			std::vector<float> colorVals = jsonGo["ambient"];
			renderer->GetMaterial().SetMaterialAmbient(XMFLOAT4(colorVals[0], colorVals[1], colorVals[2], colorVals[3]));
		}

		//Checks for the diffuse attribute and sets the objects diffuse material
		if (HasAttribute(&jsonGo, "diffuse")) {
			std::vector<float> colorVals = jsonGo["diffuse"];
			renderer->GetMaterial().SetMaterialDiffuse(XMFLOAT4(colorVals[0], colorVals[1], colorVals[2], colorVals[3]));
		}

		//Checks for the specular attribute and sets the objects specular material
		if (HasAttribute(&jsonGo, "specular")) {
			std::vector<float> colorVals = jsonGo["specular"];
			renderer->GetMaterial().SetMaterialSpecular(XMFLOAT4(colorVals[0], colorVals[1], colorVals[2], colorVals[3]));
		}
		
		//Loads the required pixel shader based off the texture maps it has
		if (hasNrmTexture) {
			renderer->SetShader(L"PhongDifSpcNrm.fx");
		}
		else if (hasSpcTexture) {
			renderer->SetShader(L"PhongDifSpc.fx");
		}
		else if (hasDifTexture) {
			renderer->SetShader(L"PhongDif.fx");
		}

		//Adds the meshed object to the gameobjects vector
		objects.emplace_back(go);
	}

	//Return the gameobjects vector 
	return objects;
}

std::vector<std::shared_ptr<Camera>> JSONLevelLoader::LoadCamerasFromFile(const char* filename)
{
	std::vector<std::shared_ptr<Camera>> cameras;

	json jsonFile = LoadJSONFile(filename);

	json cameraGroup = jsonFile["cameras"];

	int size = cameraGroup.size();
	cameras.reserve(size);

	for (int i = 0; i < size; i++) {
		std::shared_ptr<Camera> camera = std::make_shared<Camera>();
		json cameraGo = cameraGroup.at(i);

		if (HasAttribute(&cameraGo, "lookat")) {
			std::vector<float> lookatVals = cameraGo["lookat"];
			if (lookatVals.size() == 6) {
				camera->LookAt(XMFLOAT3(lookatVals[0], lookatVals[1], lookatVals[2]), XMFLOAT3(lookatVals[3], lookatVals[4], lookatVals[5]));
			}
			else if(lookatVals.size() == 3)
			{
				camera->LookAt(XMFLOAT3(lookatVals[0], lookatVals[1], lookatVals[2]), XMFLOAT3(0.0f, 0.0f, 0.0f));
			}
		}

		cameras.emplace_back(camera);
	}

	return cameras;
}

json JSONLevelLoader::LoadJSONFile(const char* filename)
{
	//Opens the file
	std::ifstream inFile(filename);

	//Creates the json reader
	json jsonFile;

	//Reads the data from the file into the json object
	inFile >> jsonFile;

	return jsonFile;
}

bool JSONLevelLoader::HasAttribute(json* file, std::string key)
{
	if (file->contains(key)) return true;

	return false;
}

wchar_t* JSONLevelLoader::ConvertString(std::string& str)
{
	//Crates a wchar_t object with the paths length + 1 allowing for the line delimiter
	wchar_t* convertedPath = new wchar_t[str.length() + 1];

	//Converts from a string to a wchar_t*
	mbstowcs_s(0, convertedPath, str.length() + 1, str.data(), _TRUNCATE);

	return convertedPath;
}
