#pragma once
#include "Graphics.h"


class Bindable
{
public:
	Bindable(ID3D11Device* device, ID3D11DeviceContext* context) : mDevice(device), mContext(context) {}
	virtual void Bind() = 0;

protected:
	ID3D11DeviceContext* mContext;
	ID3D11Device* mDevice;
};

