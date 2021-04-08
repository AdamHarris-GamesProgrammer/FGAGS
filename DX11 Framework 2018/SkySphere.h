#pragma once
#include "MeshedObject.h"

class SkySphere : public MeshedObject
{
public:
	//Constructor for the SkySphere, takes a pointer to the graphics object and a texture path
	SkySphere(Graphics* gfx, std::string& texturePath);

	//Draws the SkySphere to the screen, overrides the MeshedObject Draw method as Front face culling is needed
	void Draw() override;

	
private:
	//Converts a string to a wchar_t*, this is used for creating a texture
	wchar_t* ConvertString(std::string& str);
};

