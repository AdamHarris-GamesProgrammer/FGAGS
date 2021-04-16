#pragma once
#include "Vector3.h"

class Quaternion {
public:
	//Stores the data for the quaternion
	float _r;
	float _i;
	float _j;
	float _k;

	//Initializes all values to 0
	Quaternion() : _r(0), _i(0), _j(0), _k(0) {}

	//Initializes all values to the given values
	Quaternion(const float r, const float i, const float j, const float k) : _r(r), _i(i), _j(j), _k(k) {}

	//Normalizes the quaternion 
	void Normalize() {
		//Calculates the magnitude/length
		float d = _r * _r + _i * _i + _j * _j + _k * _k;

		//Check for 0 length
		if (d == 0) {
			_r = 1;
			return;
		}

		d = 1.0f / sqrtf(d);
		_r *= d;
		_i *= d;
		_j *= d;
		_k *= d;
	}

	//multiplies this quaternion by another
	void operator*=(const Quaternion& multiplier) {
		Quaternion q = *this;
		_r = q._r * multiplier._r - q._i * multiplier._i -
			q._j * multiplier._j - q._k * multiplier._k;
		_i = q._r * multiplier._i + q._i * multiplier._r +
			q._j * multiplier._k - q._k * multiplier._j;
		_j = q._r * multiplier._j + q._j * multiplier._r +
			q._k * multiplier._i - q._i * multiplier._k;
		_k = q._r * multiplier._k + q._k * multiplier._r +
			q._i * multiplier._j - q._j * multiplier._i;
	}

	//Rotates this quaternion by a vector
	void RotateByVector(const Vector3& vector) {
		Quaternion q(0, vector.x, vector.y, vector.z);
		(*this) *= q;
	}

	//Adds a scaled vector to this quaternion
	void AddScaledVector(const Vector3& vector, float scale) {
		Quaternion q(0, vector.x * scale, vector.y * scale, vector.z * scale);
		q *= *this;

		_r += q._r * 0.5f;
		_i += q._i * 0.5f;
		_j += q._j * 0.5f;
		_k += q._k * 0.5f;
	}

	//Returns the identity of this quaternion 
	Vector3 Identity() {
		return Vector3(_i, _j, _k);
	}
};