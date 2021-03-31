#pragma once
#include "Transform.h"
#include <vector>
#include <map>
#include "Component.h"
#include <algorithm>
#include <string>

class Object
{
public:
	Object();

	virtual void Update(float dt);
	virtual void UpdateTransform();

	Component* GetComponent(ComponentID id);

	void AddComponent(Component* component);

	Transform& GetTransform();
	std::string GetName() const;
	void SetName(std::string& name);

private:
	virtual void Initialize() = 0;

	std::map<ComponentID, Component*> _components;

protected:
	Transform _transform;
	std::string _name = "Object";

};

