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

	void SetPosition(float x, float y, float z) {
		_pTransform->SetPosition(x, y, z);
	}

	void SetRotation(float x, float y, float z) {
		_pTransform->SetRotation(x, y, z);
	}
private:
	std::map<ComponentID, Component*> _components;

protected:
	TransformComponent* _pTransform;
	std::string _name = "Object";

};

