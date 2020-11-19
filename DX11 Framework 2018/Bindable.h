#pragma once
#include "Graphics.h"


class Bindable
{
public:
	Bindable(Graphics* gfx) : mGfx(gfx) {}
	virtual void Finalize() = 0;
	virtual void Bind() = 0;

protected:
	Graphics* mGfx;
};

