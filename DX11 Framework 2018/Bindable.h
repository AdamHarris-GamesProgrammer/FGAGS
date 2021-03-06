#pragma once
#include "Graphics.h"

/// <summary>
/// Bindable Class acts as a purely virtual base class for all things that get bound to the graphical pipeline, such as Vertex Buffers, Index Buffers and Shaders
/// </summary>
/// //K2
class Bindable
{
public:
	Bindable(ID3D11Device* device, ID3D11DeviceContext* context) : _pDevice(device), _pDeviceContext(context) {}

	/// <summary>
	/// Binds the Desired object to the graphical pipeline
	/// </summary>
	virtual void Bind() = 0;

protected:
	//Used for binding the object to the pipeline
	ID3D11DeviceContext* _pDeviceContext;

	//Used for creating the object
	ID3D11Device* _pDevice;
};

