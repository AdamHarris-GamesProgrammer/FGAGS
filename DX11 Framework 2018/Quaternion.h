#pragma once
#include "Vector3.h"

class Quaternion {
public:
	float _r;
	float _i;
	float _j;
	float _k;

	Quaternion() : _r(0), _i(0), _j(0), _k(0) {}

	Quaternion(const float r, const float i, const float j, const float k) : _r(r), _i(i), _j(j), _k(k) {}


	void Normalize() {
		float d = _r * _r + _i * _i + _j * _j + _k * _k;

		//Check for 0 length
		if (d == 0) {
			_r = 1;
			return;
		}

		d = ((float)1.0) / sqrtf(d);
		_r *= d;
		_i *= d;
		_j *= d;
		_k *= d;
	}

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

	void RotateByVector(const Vector3& vector) {
		Quaternion q(0, vector.x, vector.y, vector.z);
		(*this) *= q;
	}

	void AddScaledVector(const Vector3& vector, float scale) {
		Quaternion q(0, vector.x * scale, vector.y * scale, vector.z * scale);
		q *= *this;

		_r += q._r * ((float)0.5);
		_i += q._i * ((float)0.5);
		_j += q._j * ((float)0.5);
		_k += q._k * ((float)0.5);
	}

	Vector3 Identity() {
		return Vector3(_i, _j, _k);
	}
};