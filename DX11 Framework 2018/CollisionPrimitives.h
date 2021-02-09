#pragma once
#include "Vector3.h"
#include "Rigidbody.h"

class Primitive {
public:
	Rigidbody* _body;
	Matrix4 _offset;

	void CalculateInternals() {
		_transform = _body->GetTransform() * _offset;
	}

	Vector3 GetAxis(unsigned index) const {
		return _transform.GetAxisVector(index);
	}

	const Matrix4& GetTransform() const {
		return _transform;
	}

protected:
	Matrix4 _transform;
};

class Sphere : public Primitive {
public:
	real _radius;
};

class CollisionPlane {
public:
	Vector3 _direction;
	real _offset;
};

class Box : public Primitive {
public:
	Vector3 _halfSize;
};