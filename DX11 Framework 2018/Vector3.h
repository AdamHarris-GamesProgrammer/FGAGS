#pragma once

#include <DirectXMath.h>
#include "Debug.h"

using namespace DirectX;

class Vector3 {
public:
	//data for the vector
	float x, y, z;

private:
	//Padding is here as it adds an additional byte for WORD alignment, more efficient on memory
	float pad;

public:
	//Initialize to zero
	Vector3() : x(0), y(0), z(0), pad(0) {}

	//initializes the vector to the given values
	Vector3(const float x, const float y, const float z)
		: x(x), y(y), z(z), pad(0) {}

	//Calculates the magnitude of the vector
	float Magnitude() const {
		return sqrtf(x * x + y * y + z * z);
	}

	//Calculates the squared magnitude of the vector this is more efficient than conventional magnitude as it does not use a square root operation
	float SquareMagnitude() const {
		return x * x + y * y + z * z;
	}

	//Returns the distance between this and the given vector
	float Distance(const Vector3& vec) {
		return sqrtf(powf(x - vec.x, 2) + powf(y - vec.y, 2) + powf(z - vec.z, 2));
	}

	//Normalizes this vector
	void Normalize() {
		float mag = Magnitude();
		if (mag > 0) {
			(*this) *= ((float)1) / mag;
		}
	}

	//Multiplies this vector by a scaler value
	void operator*=(const float val) {
		x *= val;
		y *= val;
		z *= val;
	}

	//multiplies this vector by a scaler value
	Vector3 operator*(const float val) const{
		return Vector3(x * val, y * val, z * val);
	}

	//Adds another vector to this vector
	void operator+=(const Vector3& vec) {
		x += vec.x;
		y += vec.y;
		z += vec.z;
	}

	//Adds a vector to this vector and returns it as a new vector
	Vector3 operator+(const Vector3& vec) const{
		return Vector3(x + vec.x, y + vec.y, z + vec.z);
	}

	//checks if the provided vector is not the same as this vector
	bool operator!=(const Vector3& vec) const {
		return (x != vec.x || y != vec.y || z != vec.z);
	}

	//Subtracts another vector from this one
	void operator-=(const Vector3 & vec) {
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
	}

	//Subtracts another vector from this one and returns it as a new vector
	Vector3 operator-(const Vector3& vec) const {
		return Vector3(x - vec.x, y - vec.y, z - vec.z);
	}

	//Adds a scaled vector to this same as saying vec = vecB * scalar. just a shorthand
	void AddScaledVector(const Vector3& vec, float scale) {
		x += vec.x * scale;
		y += vec.y * scale;
		z += vec.z * scale;
	}

	//Returns the product of this vector and the given vectors components
	Vector3 ComponentProduct(const Vector3& vec) const {
		return Vector3(x * vec.x, y * vec.y, z * vec.z);
	}

	//returns the scalar product of this vector and another vector
	float ScalarProduct(const Vector3& vec) const {
		return x * vec.x + y * vec.y + z * vec.z;
	}

	//multiplies this vector by another and returns a float
	float operator*(const Vector3& vec) const {
		return x * vec.x + y * vec.y + z * vec.z;
	}

	//Returns a vector for the vector product of this vector and a given vector
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

	float operator[](unsigned i) const
	{
		if (i == 0) return x;
		if (i == 1) return y;
		return z;
	}

	float& operator[](unsigned i)
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

	void ComponentProductUpdate(const Vector3& vec) {
		x *= vec.x;
		y *= vec.y;
		z *= vec.z;
	}

	//Sets the components of this vector to the provided values
	void Set(float newX, float newY, float newZ) {
		this->x = newX;
		this->y = newY;
		this->z = newZ;
	}

	//Outputs this vector to the output window
	void Print(std::string name) {
		Debug::Print(name + "X: %f ", x);
		Debug::Print("Y: %f ", y);
		Debug::Print("Z: %f", z);
		Debug::Print("\n");
	}

	bool operator<(float val) {
		return (x < val && y < val && z < val);
	}

	bool operator>(float val) {
		return (x > val && y > val && z > val);
	}

	bool operator==(const Vector3& other) {
		return(x == other.x && y == other.y && z == other.z);
	}

	bool operator <=(const Vector3& other) {
		return(x <= other.x && y <= other.y && z <= other.z);
	}
};