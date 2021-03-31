#include "Object.h"

Object::Object()
{
	_transform = Transform();
	_name = "Object";
}

void Object::Update(float dt)
{

}

void Object::UpdateTransform()
{
	_transform.Update();
}

Component* Object::GetComponent(ComponentID id)
{
	auto search = _components.find(id);

	if (search != _components.end()) {
		return search->second;
	}
	else
	{
		return nullptr;
	}
}

bool sortByUpdate(std::pair<ComponentID, Component*> a, std::pair<ComponentID, Component*> b) {
	return a.second->GetUpdatePriority() < b.second->GetUpdatePriority();
}

void Object::AddComponent(Component* component)
{
	_components.insert({ component->GetID(), component });

	std::vector<std::pair<ComponentID, Component*>> comp;
	for (auto& it : _components) {
		comp.push_back(it);
	}

	std::sort(comp.begin(), comp.end(), sortByUpdate);
}

Transform& Object::GetTransform()
{
	return _transform;
}

std::string Object::GetName() const
{
	return _name;
}

void Object::SetName(std::string& name)
{
	_name = name;
}

void Object::Initialize()
{

}
