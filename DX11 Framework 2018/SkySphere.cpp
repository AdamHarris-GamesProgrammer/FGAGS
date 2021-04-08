#include "SkySphere.h"

SkySphere::SkySphere(Graphics* gfx, std::string& texturePath)
	: MeshedObject(gfx, "Assets/Models/sphere.obj")
{
	//Makes the SkySphere large
	_pTransform->SetScale(500.0f, 500.0f, 500.0f);

	//Creates the texture from the provided path
	CreateTexture(ConvertString(texturePath));

	//Sets the shader for the skysphere
	SetShader(L"Skybox.fx");

	//Names the SkySphere
	_name = "SkySphere";
}

void SkySphere::Draw()
{
	//Sets front face culling for this frame
	_pGfx->SetFrontFaceCulling();

	//Draws our object
	MeshedObject::Draw();

	//Sets back to default draw state
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
