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
	ComponentID _id;
	float _updatePriority;

protected:
	Object* _pOwner = nullptr;

};

#endif // !COMPONENT_H