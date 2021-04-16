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

	//Adds a component
	void AddComponent(Component* component);

	//Removes a component based on its id
	void RemoveComponent(ComponentID id);

	//Returns the transform component
	TransformComponent& GetTransform();
	std::string GetName() const;
	void SetName(std::string& name);
private:
	std::map<ComponentID, Component*> _components;

protected:
	//Every object will need a transform
	TransformComponent* _pTransform;
	std::string _name = "Object";

};

