#pragma once
#ifndef COMPONENT_H
#define COMPONENT_H

#include "ComponentIDs.h"


class Object;

class Component {
public:
	Component(Object* owner, ComponentID id, float updatePriority);
	~Component();

	//This method defines what the component will do each frame
	virtual void Update(float dt) = 0;

	//Returns the Component ID 
	ComponentID GetID() const;

	//Changes the owner of the component, acts as a way to not recreate multiple components
	virtual void SetOwner(Object* newOwner);

	//Returns the update priority of the component
	float GetUpdatePriority() const;

private:
	//Id for the component
	ComponentID _id;
	//Update priority, lower update priority means it will be updated first
	float _updatePriority;

protected:
	//Stores a pointer to the owning object
	Object* _pOwner = nullptr;

};

#endif // !COMPONENT_H