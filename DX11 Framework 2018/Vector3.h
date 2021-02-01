#pragma once

#include "Precision.h"

class Vector3 {
public:
	real x, y, z;

private:
	real pad;

public:
	Vector3() : x(0), y(0), z(0) {}

	Vector3(const real x, const real y, const real z)
		: x(x), y(y), z(z) {}

	void Invert() {
		x = -x;
		y = -y;
		z = -z;
	}

	real Magnitude() const {
		return real_sqrt(x * x + y * y + z * z);
	}

	real SquareMagnitude() const {
		return x * x + y * y + z * z;
	}

	void Normalize() {
		real mag = Magnitude();
		if (mag > 0) {
			(*this) *= ((real)1) / mag;
		}
	}

	void operator*=(const real val) {
		x *= val;
		y *= val;
		z *= val;
	}

	Vector3 operator*(const real val) const{
		return Vector3(x * val, y * val, z * val);
	}

	void operator+=(const Vector3& vec) {
		x += vec.x;
		y += vec.y;
		z += vec.z;
	}

	Vector3 operator+(const Vector3& vec) const{
		return Vector3(x + vec.x, y + vec.y, z + vec.z);
	}

	void operator-=(const Vector3 & vec) {
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
	}

	Vector3 operator-(const Vector3& vec) const {
		return Vector3(x - vec.x, y - vec.y, z - vec.z);
	}

	void AddScaledVector(const Vector3& vec, real scale) {
		x += vec.x * scale;
		y += vec.y * scale;
		z += vec.z * scale;
	}

	Vector3 ComponentProduct(const Vector3& vec) const {
		return Vector3(x * vec.x, y * vec.y, z * vec.z);
	}

	void ComponentProductUpdate(const Vector3& vec) {
		x *= vec.x;
		y *= vec.y;
		z *= vec.z;
	}

	real ScalarProduct(const Vector3& vec) const {
		return x * vec.x + y * vec.y + z * vec.z;
	}

	real operator*(const Vector3& vec) const {
		return x * vec.x + y * vec.y + z * vec.z;
	}

	Vector3 VectorProduct(const Vector3& vec) const {
		return Vector3(y * vec.z - z * vec.y,
					   z * vec.x - x * vec.z,
					   x * vec.y - y * vec.x);
	}

	void operator%=(const Vector3& vec) {
		*this = VectorProduct(vec);
	}

	Vector3 operator%(const Vector3& vec) {
		return Vector3(y * vec.z - z * vec.y,
					   z * vec.x - x * vec.z,
					   x * vec.y - y * vec.x);
	}

	void MakeOrthonormalBasis(Vector3* a, Vector3* b, Vector3* c) {
		a->Normalize();
		(*c) = (*a) % (*b);
		if (c->SquareMagnitude() == 0.0) return;
		c->Normalize();
		(*b) = (*c) % (*a);
	}
};