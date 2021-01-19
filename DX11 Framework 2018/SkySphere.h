#pragma once
#include "MeshedObject.h"

class SkySphere : public MeshedObject
{
public:
	SkySphere(Graphics* gfx, std::string& texturePath);

	void Update(float dt) override;
	void Draw() override;

	
private:
	wchar_t* ConvertString(std::string& str);
};

