#pragma once
#include "Graphics.h"

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_dx11.h"
#include "Imgui/imgui_impl_win32.h"

class ImGUIManager
{
public:
	ImGUIManager() = default;
	ImGUIManager(Graphics* gfx);


	void BeginFrame();

	void EndFrame();

private:
	void SetStyle();
};

