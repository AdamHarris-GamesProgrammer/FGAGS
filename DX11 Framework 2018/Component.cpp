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
	//Removes this component from the current owner
	_pOwner->RemoveComponent(_id);
	//switches to new owner
	_pOwner = newOwner;

	//Adds this component to the new owner
	_pOwner->AddComponent(this);
}

float Component::GetUpdatePriority() const
{
	return _updatePriority;
}
