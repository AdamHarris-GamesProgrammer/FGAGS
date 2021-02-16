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
