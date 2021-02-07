#pragma once
#include "Precision.h"
#include "Vector3.h"

class Quaternion {
public:
	union 
	{
		struct 
		{
			real _r;
			real _i;
			real _j;
			real _k;
		};

		real _data[4];
	};

	Quaternion() : _r(0), _i(0), _j(0), _k(0) {}

	Quaternion(const real r, const real i, const real j, const real k) : _r(r), _i(i), _j(j), _k(k) {}


	void Normalize() {
		real d = _r * _r + _i * _i + _j * _j + _k * _k;

		//Check for 0 length
		if (d == 0) {
			_r = 1;
			return;
		}

		d = ((real)1.0) / real_sqrt(d);
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

	void AddScaledVector(const Vector3& vector, real scale) {
		Quaternion q(0, vector.x * scale, vector.y * scale, vector.z * scale);
		q *= *this;

		_r += q._r * ((real)0.5);
		_i += q._i * ((real)0.5);
		_j += q._j * ((real)0.5);
		_k += q._k * ((real)0.5);
	}
};