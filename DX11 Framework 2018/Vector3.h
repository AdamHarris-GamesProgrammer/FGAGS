#pragma once

#include "Precision.h"
#include <DirectXMath.h>
#include "Debug.h"

using namespace DirectX;

class Vector3 {
public:
	real x, y, z;

private:
	real pad;

public:
	Vector3() : x(0), y(0), z(0), pad(0) {}

	Vector3(const real x, const real y, const real z)
		: x(x), y(y), z(z), pad(0) {}

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

	real Distance(const Vector3& vec) {
		return real_sqrt(real_pow(x - vec.x, 2) + real_pow(y - vec.y, 2) + real_pow(z - vec.z, 2));
	}

	real DistanceSquared(const Vector3& vec) {
		return real_pow(x - vec.x, 2) + real_pow(y - vec.y, 2) + real_pow(z - vec.z, 2);
	}

	real XZDistance(const Vector3& vec) {
		return real_sqrt(real_pow(x - vec.x, 2) + real_pow(z - vec.z, 2));
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

	real operator[](unsigned i) const
	{
		if (i == 0) return x;
		if (i == 1) return y;
		return z;
	}

	real& operator[](unsigned i)
	{
		if (i == 0) return x;
		if (i == 1) return y;
		return z;
	}

	operator XMFLOAT3() const {
		return XMFLOAT3(x, y, z);
	}

	void Zero() {
		x = 0.0;
		y = 0.0;
		z = 0.0;
	}

	void Print(std::string name) {
		Debug::Print(name + "X: %f ", x);
		Debug::Print("Y: %f ", y);
		Debug::Print("Z: %f", z);
		Debug::Print("\n");
	}

	bool operator<(real val) {
		return (x < val && y < val && z < val);
	}

	bool operator>(real val) {
		return (x > val && y > val && z > val);
	}

	bool operator==(const Vector3& other) {
		return(x == other.x && y == other.y && z == other.z);
	}

	bool operator <=(const Vector3& other) {
		return(x <= other.x && y <= other.y && z <= other.z);
	}
};