#include "Scene.h"
#include <thread>
#include <sstream>

Scene::Scene(Graphics* gfx, const char* filename) : _pGfx(gfx), _pLevelFile(filename)
{
	Initialize();
}

Scene::~Scene()
{
	//Application class handles deleting the Graphics object pointer
	_pGfx = nullptr;

	//Clears the Camera and Object vectors
	_pCameras.clear();
	_pGameObjects.clear();
}

void Scene::LoadLevel()
{
	//Clears the memory associated with our GameObject and Camera vectors
	_pGameObjects.clear();
	_pCameras.clear();

	//Starts up two threads, one for object loading and one for camera loading
	std::thread objectLoader(&Scene::LoadObjectsFromFile, this, _pLevelFile);
	std::thread cameraLoader(&Scene::LoadCameraObjectsFromFile, this, _pLevelFile);

	//Waits for the camera thread to be done
	cameraLoader.join();
	_pCurrentCamera = _pCameras[0];
	_pGfx->SetCurrentCamera(_pCurrentCamera);

	//Waits for the object thread to be done
	objectLoader.join();
}

void Scene::ExitLevel()
{
	_pGameObjects.clear();
	_pCameras.clear();
}

void Scene::PollInput(float dt)
{
	//Resets the scene
	if (GetAsyncKeyState('R')) {
		Reset();
	}
}

void Scene::BeginFrame(float dt)
{
	//Begins our frame (mainly ImGUI prep) and wiping buffers
	_pGfx->BeginFrame();

	//Updates the scene camera
	_pCurrentCamera->Update(dt);

	//Updates the objects in the 
	for (auto& object : _pGameObjects) {
		object->Update(dt);
	}

	//Draws the level UI
	DrawUI();
}

void Scene::EndFrame()
{
	//Ends our frame (swaps framebuffers etc)
	_pGfx->EndFrame();
}

void Scene::Reset()
{
	LoadLevel();
}

void Scene::FlyCamera(float dt)
{
	//Move forward
	if (GetAsyncKeyState('W')) _pCameras[0]->Walk(10.0f * dt);
	//Move backward
	else if (GetAsyncKeyState('S')) _pCameras[0]->Walk(-10.0f * dt);
	//Move left
	if (GetAsyncKeyState('A')) _pCameras[0]->Strafe(-10.0f * dt);
	//Move right
	else if (GetAsyncKeyState('D')) _pCameras[0]->Strafe(10.0f * dt);

	//Rotate left
	if (GetAsyncKeyState('Q')) _pCameras[0]->RotateY(-1.0f * dt);
	//Rotate right
	else if (GetAsyncKeyState('E')) _pCameras[0]->RotateY(1.0f * dt);
	//Rotate upwards
	if (GetAsyncKeyState('R')) _pCameras[0]->Pitch(-1.0f * dt);
	//Rotate downwards
	else if (GetAsyncKeyState('F')) _pCameras[0]->Pitch(1.0f * dt);
}

void Scene::ResetButton()
{
	if (ImGui::Button("Reset Simulation")) {
		Reset();
	}
}

void Scene::OutputVector3(std::string label, Vector3 vec)
{
	//Creates a ostringstream object for simplifying outputting the vector
	std::ostringstream output;
	//Sets it to six significant figures, for the sakes of this project this is fine. However after 9999.99 it will go down to 1dp
	output.precision(6);
	//Prepares the ostringstream
	output << label << " {X: " << vec.x << " Y: " << vec.y << " Z: " << vec.z << "}";
	//Converts the output to a const char* for ImGUI
	ImGui::Text(output.str().c_str());
}

void Scene::LoadGround()
{
	if (_pGroundPlane == nullptr) {
		_pGroundPlane = std::make_unique<Object>();
	}

	if (_pGroundPlaneRenderer == nullptr) {
		_pGroundPlaneRenderer = std::make_unique<RendererComponent>(_pGroundPlane.get(), _pGfx);

	}

	MakePlane(20.0f, 20.0f, 7, 7, _pGroundPlaneRenderer.get());

	_pGroundPlaneRenderer->CreateTexture(L"Assets/Textures/stone.dds");

	_pGameObjects.push_back(_pGroundPlane.get());
}

void Scene::Initialize()
{
	//Create memory for the level loader
	_pLevelLoader = std::make_unique<JSONLevelLoader>();
}

void Scene::LoadObjectsFromFile(const char* filename)
{
	_pGameObjects = _pLevelLoader->LoadObjectsFromFile(filename, _pGfx);
}

void Scene::LoadCameraObjectsFromFile(const char* filename)
{
	_pCameras = _pLevelLoader->LoadCamerasFromFile(filename);
}

void Scene::MakePlane(float width, float depth, UINT m, UINT n, RendererComponent* renderer)
{
	//Calculates the amount of vertices and faces required for the plane
	UINT vertexCount = m * n;
	UINT faceCount = (m - 1) * (n - 1) * 2;

	//Creates vectors for both of the vertices and the indices
	std::vector<SimpleVertex> verts;
	std::vector<WORD> indices;

	//Calculates half of the width and depth
	float halfWidth = 0.5f * width;
	float halfDepth = 0.5f * depth;

	//Calculates the delta between each row
	float dx = width / (n - 1);
	//Calculates the delta between each column
	float dz = depth / (m - 1);

	//Calculates the delta between each row and column of tex coords
	float du = 1.0f / (n - 1);
	float dv = 1.0f / (m - 1);

	//Resizes the vertices vector reducing overhead from not calling copy constructor
	verts.resize(vertexCount);

	//Loops through the amount of rows
	for (UINT i = 0; i < m; ++i)
	{
		//Calculates the z position value
		float z = halfDepth - i * dz;
		//Loops through the amount of columns
		for (UINT j = 0; j < n; ++j)
		{
			//Calculates the X position value
			float x = -halfWidth + j * dx;

			//Sets the normals and positions of the vertex
			verts[i * n + j].Pos = XMFLOAT3(x, 0.0f, z);
			verts[i * n + j].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

			// Stretch texture over grid.
			verts[i * n + j].TexC.x = j * du;
			verts[i * n + j].TexC.y = i * dv;
		}
	}

	renderer->CreateVertexBuffer(verts);
	verts.clear();

	//Resizes the indices vector to avoid calling the copy constructor for every new index
	indices.resize(faceCount * 3); // 3 indices per face

	// Iterate over each quad and compute indices.
	UINT k = 0;
	for (UINT i = 0; i < m - 1; ++i)
	{
		for (UINT j = 0; j < n - 1; ++j)
		{
			indices[k] = i * n + j;
			indices[k + 1] = i * n + j + 1;
			indices[k + 2] = (i + 1) * n + j;

			indices[k + 3] = (i + 1) * n + j;
			indices[k + 4] = i * n + j + 1;
			indices[k + 5] = (i + 1) * n + j + 1;

			k += 6; // next quad
		}
	}
	//Creates the index buffer
	renderer->CreateIndexBuffer(indices);
	indices.clear();

	//Sets the shader
	renderer->SetShader(L"PhongDif.fx");
}
