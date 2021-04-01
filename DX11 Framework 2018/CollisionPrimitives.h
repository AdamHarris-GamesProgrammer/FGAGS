#pragma once
#include "Vector3.h"
#include "Rigidbody.h"

class CollisionPrimitive {
public:
	RigidbodyComponent* _body;
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
	Sphere(RigidbodyComponent* body, float radius = 1.0f) {
		_body = body;
		_radius = radius;
		CalculateInternals();
	}

	float _radius;
};

class CollisionPlane {
public:
	CollisionPlane(Vector3 direction, float offset) {
		_direction = direction;
		_offset = offset;
	}

	Vector3 _direction;
	float _offset = 0.0f;
};

class Box : public CollisionPrimitive {
public:
	Box(RigidbodyComponent* body, Vector3 halfSize) {
		_body = body;
		_halfSize = halfSize;
		CalculateInternals();
	}

	Box(RigidbodyComponent* body) {
		_body = body;
		_halfSize = Vector3(1.0f, 1.0f, 1.0f);
		CalculateInternals();
	}

	Vector3 _halfSize;
};