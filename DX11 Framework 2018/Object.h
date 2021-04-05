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
	~Object();

	virtual void Update(float dt);

	Component* GetComponent(ComponentID id);

	void AddComponent(Component* component);
	void RemoveComponent(ComponentID id);

	TransformComponent& GetTransform();
	std::string GetName() const;
	void SetName(std::string& name);

private:
	virtual void Initialize() = 0;

	std::map<ComponentID, Component*> _components;

protected:
	TransformComponent* _pTransform;
	std::string _name = "Object";

};

