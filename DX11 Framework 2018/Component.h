#pragma once
#ifndef COMPONENT_H
#define COMPONENT_H

#include "ComponentIDs.h"


class Object;

class Component {
public:
	Component(Object* owner, ComponentID id, float updatePriority);

	virtual void Update(float dt) = 0;

	ComponentID GetID() const;

	float GetUpdatePriority() const;

private:
	Object* _pOwner = nullptr;
	ComponentID _id;
	float _updatePriority;

};

#endif // !COMPONENT_H