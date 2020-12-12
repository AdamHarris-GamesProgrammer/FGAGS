#pragma once

#include <Windows.h>
#include <DirectXMath.h>

#include "Imgui/imgui.h"

using namespace DirectX;

struct Light {
	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;

	void ColourControls() {
		float diffuse[4] = {
			Diffuse.x,
			Diffuse.y,
			Diffuse.z,
			Diffuse.w
		};

		float ambient[4] = {
			Ambient.x,
			Ambient.y,
			Ambient.z,
			Ambient.w
		};

		float specular[3] = {
			Specular.x,
			Specular.y,
			Specular.z
		};

		ImGui::Text("Diffuse");
		ImGui::SameLine();
		ImGui::ColorEdit4("#", diffuse);

		ImGui::Text("Ambient");
		ImGui::SameLine();
		ImGui::ColorEdit4("##", ambient);

		ImGui::Text("Specular");
		ImGui::SameLine();
		ImGui::ColorEdit3("###", specular);

		Diffuse = XMFLOAT4(diffuse[0], diffuse[1], diffuse[2], diffuse[3]);
		Ambient = XMFLOAT4(ambient[0], ambient[1], ambient[2], ambient[3]);

		float specularPower = Specular.w;
		Specular = XMFLOAT4(specular[0], specular[1], specular[2], specularPower);
	}
};

struct DirectionalLight : Light {
	DirectionalLight() { ZeroMemory(this, sizeof(this)); }


	XMFLOAT3 Direction;
	float Padding;

	void Controls() {
		if (ImGui::TreeNode("Colour Settings")) {
			ColourControls();
			ImGui::TreePop();
		}

		ImGui::Text("Direction");
		ImGui::SameLine();

		float val[3] = {
			Direction.x, Direction.y, Direction.z
		};

		ImGui::SliderFloat3("####", val, -1.0f, 1.0f);

		Direction = XMFLOAT3(val[0], val[1], val[2]);

		ImGui::TreePop();
	}
};

struct PointLight : Light {
	PointLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT3 Position;
	float Range;

	XMFLOAT3 Attenuation;
	float Padding;

	void Controls() {
		if (ImGui::TreeNode("Colour Settings")) {
			ColourControls();
			ImGui::TreePop();
		}

		ImGui::Text("Position");
		ImGui::SameLine();

		float tempPos[3] = {
			Position.x, Position.y, Position.z
		};

		ImGui::SliderFloat3("####", tempPos, -100.0f, 100.0f);

		Position = XMFLOAT3(tempPos[0], tempPos[1], tempPos[2]);


		ImGui::Text("Range");
		ImGui::SameLine();

		ImGui::SliderFloat("######", &Range, 0.1f, 100.0f);


		ImGui::Text("Attenuation");
		ImGui::SameLine();
		float tempAttenuation[3] = {
			Attenuation.x, Attenuation.y, Attenuation.z
		};

		ImGui::SliderFloat3("#####", tempAttenuation, 0.0f, 1.0f);

		Attenuation = XMFLOAT3(tempAttenuation[0], tempAttenuation[1], tempAttenuation[2]);


		ImGui::TreePop();
	}
};

struct SpotLight : Light {
	SpotLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT3 Position;
	float Range;

	XMFLOAT3 Direction;
	float Spot;

	XMFLOAT3 Attenuation;
	float Padding;

	void Controls() {
		if (ImGui::TreeNode("Colour Settings")) {
			ColourControls();
			ImGui::TreePop();
		}

		ImGui::Text("Position");
		ImGui::SameLine();

		float tempPos[3] = {
			Position.x, Position.y, Position.z
		};

		ImGui::SliderFloat3("####", tempPos, -100.0f, 100.0f);

		Position = XMFLOAT3(tempPos[0], tempPos[1], tempPos[2]);

		ImGui::Text("Range");
		ImGui::SameLine();

		ImGui::SliderFloat("######", &Range, 0.1f, 100.0f);

		ImGui::Text("Attenuation");
		ImGui::SameLine();
		float tempAttenuation[3] = {
			Attenuation.x, Attenuation.y, Attenuation.z
		};

		ImGui::SliderFloat3("#####", tempAttenuation, 0.0f, 1.0f);

		Attenuation = XMFLOAT3(tempAttenuation[0], tempAttenuation[1], tempAttenuation[2]);

		ImGui::Text("Spot Exponent");
		ImGui::SameLine();
		ImGui::SliderFloat("#######", &Spot, 0.01f, 100.0f);

		ImGui::TreePop();
	}
};

struct Material : Light {
	Material() { ZeroMemory(this, sizeof(this)); }
};