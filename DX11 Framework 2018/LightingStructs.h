#pragma once

#include <DirectXMath.h>

struct BasicLight
{
	BasicLight() = default;
	BasicLight(DirectX::XMFLOAT4 mat, DirectX::XMFLOAT4 lig) {
		material = mat;
		light = lig;
	}

	DirectX::XMFLOAT4 material;
	DirectX::XMFLOAT4 light;
};

struct LightWithIntensity : BasicLight {
	LightWithIntensity() = default;
	LightWithIntensity(DirectX::XMFLOAT4 mat, DirectX::XMFLOAT4 lig, float power) : BasicLight(mat, lig) {
		intensity = power;
	}

	float intensity;
};