#include "GfxResources.h"

ID3D11DeviceContext* GfxResources::GetContext(Graphics& gfx) noexcept
{
	return gfx._pImmediateContext.Get();
}

ID3D11Device* GfxResources::GetDevice(Graphics& gfx) noexcept
{
	return gfx._pd3dDevice.Get();
}
