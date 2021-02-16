#pragma once
#include "Transform.h"
#include <string>

class Object
{
public:
	Object();

	virtual void Update(float dt);
	virtual void UpdateTransform();

	Transform& GetTransform();
	std::string GetName() const;
	void SetName(std::string& name);

private:
	virtual void Initialize() = 0;

protected:
	Transform _transform;
	std::string _name = "Object";

};

