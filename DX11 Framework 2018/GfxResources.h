#pragma once
#include "Graphics.h"

class Graphics;

class GfxResources{
protected:
	static ID3D11DeviceContext* GetContext(Graphics& gfx) noexcept;
	static ID3D11Device* GetDevice(Graphics& gfx) noexcept;

};

