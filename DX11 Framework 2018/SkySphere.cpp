#include "SkySphere.h"

SkySphere::SkySphere(Graphics* gfx, std::string& texturePath)
	: MeshedObject(gfx, "Assets/Models/sphere.obj")
{
	_pTransform->SetScale(500.0f, 500.0f, 500.0f);
	CreateTexture(ConvertString(texturePath));
	SetShader(L"Skybox.fx");
	_name = "SkySphere";
}

void SkySphere::Update(float dt)
{
	MeshedObject::Update(dt);
}

void SkySphere::Draw()
{
	_pGfx->SetFrontFaceCulling();

	MeshedObject::Draw();

	_pGfx->SetCurrentRSState();
}

wchar_t* SkySphere::ConvertString(std::string& str)
{
	//Crates a wchar_t object with the paths length + 1 allowing for the line delimiter
	wchar_t* convertedPath = new wchar_t[str.length() + 1];

	//Converts from a string to a wchar_t*
	mbstowcs_s(0, convertedPath, str.length() + 1, str.data(), _TRUNCATE);

	return convertedPath;
}
