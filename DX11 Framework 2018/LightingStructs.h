#pragma once

#include <Windows.h>
#include <DirectXMath.h>

using namespace DirectX;

struct Light {
	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
};

struct DirectionalLight : Light {
	DirectionalLight() { ZeroMemory(this, sizeof(this)); }


	XMFLOAT3 Direction;
	float Padding;
};

struct PointLight : Light {
	PointLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT3 Position;
	float Range;

	XMFLOAT3 Attenuation;
	float Padding;
};

struct SpotLight : Light {
	SpotLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT3 Position;
	float Range;

	XMFLOAT3 Direction;
	float Spot;

	XMFLOAT3 Attenuation;
	float Padding;
};

struct Material : Light {
	Material() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 Reflect;
};