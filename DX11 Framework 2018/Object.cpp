#include "Object.h"

Object::Object()
{
	_pTransform = new TransformComponent(this);
	_name = "Object";
}

Object::~Object()
{
	_components.clear();
}

void Object::Update(float dt)
{
	//updates our components
	for (auto& component : _components) {
		component.second->Update(dt);
	}
}

Component* Object::GetComponent(ComponentID id)
{
	//performs a search for our desired id
	auto search = _components.find(id);

	//checks if we have the component
	if (search != _components.end()) {
		//returns the component
		return search->second;
	}
	else
	{
		return nullptr;
	}
}

//Sorts the update priority of components in ascending order.
bool sortByUpdate(std::pair<ComponentID, Component*> a, std::pair<ComponentID, Component*> b) {
	return a.second->GetUpdatePriority() < b.second->GetUpdatePriority();
}

void Object::AddComponent(Component* component)
{
	//inserts the new component into the map
	_components.insert({ component->GetID(), component });

	//creates a vector and clones the map into the vector using references
	std::vector<std::pair<ComponentID, Component*>> comp;
	for (auto& it : _components) {
		comp.push_back(it);
	}

	//sorts the vector, as this is a reference to the map it sorts the map as well
	std::sort(comp.begin(), comp.end(), sortByUpdate);
}

void Object::RemoveComponent(ComponentID id)
{
	//Searches for the id and removes it from the map
	_components.erase(id);
}

TransformComponent& Object::GetTransform()
{
	return *_pTransform;
}

std::string Object::GetName() const
{
	return _name;
}

void Object::SetName(std::string& name)
{
	_name = name;
}

