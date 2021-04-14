#pragma once
#include "Vector3.h"
#include "Rigidbody.h"

/// <summary>
/// Base class for collision objects
/// </summary>
class CollisionPrimitive {
public:
	RigidbodyComponent* _body = nullptr;
	Matrix4 _offset;

	void CalculateInternals() {
		//Calculates the transform based on the rigidbody and the offset
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

/// <summary>
/// A Sphere collider class
/// </summary>
class Sphere : public CollisionPrimitive {
public:
	Sphere(RigidbodyComponent* body, float radius = 1.0f) {
		_body = body;
		_radius = radius;
		CalculateInternals();
	}

	//A sphere needs to know it's radius to detect collisions
	float _radius;
};

/// <summary>
/// A plane collision class
/// </summary>
class CollisionPlane {
public:
	CollisionPlane(Vector3 direction, float offset) {
		_direction = direction;
		_offset = offset;
	}

	//A plane will be seen as a infinitely large plane going along a certain direction
	Vector3 _direction;
	float _offset = 0.0f;
};

/// <summary>
/// A Box collision class
/// </summary>
class Box : public CollisionPrimitive {
public:
	Box(RigidbodyComponent* body, Vector3 halfSize) {
		_body = body;
		_halfSize = halfSize;
		CalculateInternals();
	}

	Box(RigidbodyComponent* body) {
		_body = body;
		//Assumes the cube is 2 units wide, tall and deep
		_halfSize = Vector3(1.0f, 1.0f, 1.0f);
		CalculateInternals();
	}

	//In order for a box to detect collisions it has to know the extents of itself.
	Vector3 _halfSize;

};