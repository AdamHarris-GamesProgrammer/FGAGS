#pragma once
#include "Vector3.h"
#include "Rigidbody.h"

class CollisionPrimitive {
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

class Sphere : public CollisionPrimitive {
public:
	float _radius;
};

class CollisionPlane {
public:
	Vector3 _direction;
	float _offset = 0.0f;
};

class Box : public CollisionPrimitive {
public:
	Vector3 _halfSize;
};