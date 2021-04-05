#include "Component.h"
#include "Object.h"

Component::Component(Object* owner, ComponentID id, float updatePriority)
	: _pOwner(owner), _id(id), _updatePriority(updatePriority)
{
	_pOwner->AddComponent(this);
}

Component::~Component()
{
	_pOwner->RemoveComponent(_id);
}

ComponentID Component::GetID() const
{
	return _id;
}

void Component::SetOwner(Object* newOwner)
{
	//TODO: Make a remove component method

	_pOwner = newOwner;
	_pOwner->AddComponent(this);
}

float Component::GetUpdatePriority() const
{
	return _updatePriority;
}
