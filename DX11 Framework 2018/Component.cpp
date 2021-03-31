#include "Component.h"
#include "Object.h"

Component::Component(Object* owner, ComponentID id, float updatePriority)
	: _pOwner(owner), _id(id), _updatePriority(updatePriority)
{
	_pOwner->AddComponent(this);
}

ComponentID Component::GetID() const
{
	return _id;
}

float Component::GetUpdatePriority() const
{
	return _updatePriority;
}
