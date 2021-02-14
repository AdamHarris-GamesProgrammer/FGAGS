#pragma once
#include "Vector3.h"
#include "Quaternion.h"

//Holds a 3x4 matrix
class Matrix4 {
public:
	//Holds the data for the matrix in array form
	float _data[12];

	Vector3 operator*(const Vector3& vector) const {
		return Vector3(
			vector.x * _data[0] + vector.y * _data[1] + vector.z * _data[2] + _data[3],
			vector.x * _data[4] + vector.y * _data[5] + vector.z * _data[6] + _data[7],
			vector.x * _data[8] + vector.y * _data[9] + vector.z * _data[10] + _data[11]
		);
	}

	Matrix4() {
		_data[1] = _data[2] = _data[3] = _data[4] = _data[6] = _data[7] = _data[8] = _data[9] = _data[11] = 0;
		_data[0] = _data[5] = _data[10] = 1;
	}

	Vector3 Transform(const Vector3& vector) const {
		return (*this) * vector;
	}

	Vector3 GetAxisVector(int i) const {
		return Vector3(_data[i], _data[i + 4], _data[i + 8]);
	}

	Matrix4 operator*(const Matrix4& o) const {
		Matrix4 result;
		result._data[0] = o._data[0] * _data[0] + o._data[4] * _data[1] +
			o._data[8] * _data[2];
		result._data[4] = o._data[0] * _data[4] + o._data[4] * _data[5] +
			o._data[8] * _data[6];
		result._data[8] = o._data[0] * _data[8] + o._data[4] * _data[9] +
			o._data[8] * _data[10];
		result._data[1] = o._data[1] * _data[0] + o._data[5] * _data[1] +
			o._data[9] * _data[2];
		result._data[5] = o._data[1] * _data[4] + o._data[5] * _data[5] +
			o._data[9] * _data[6];
		result._data[9] = o._data[1] * _data[8] + o._data[5] * _data[9] +
			o._data[9] * _data[10];
		result._data[2] = o._data[2] * _data[0] + o._data[6] * _data[1] +
			o._data[10] * _data[2];
		result._data[6] = o._data[2] * _data[4] + o._data[6] * _data[5] +
			o._data[10] * _data[6];
		result._data[10] = o._data[2] * _data[8] + o._data[6] * _data[9] +
			o._data[10] * _data[10];
		result._data[3] = o._data[3] * _data[0] + o._data[7] * _data[1] +
			o._data[11] * _data[2] + _data[3];
		result._data[7] = o._data[3] * _data[4] + o._data[7] * _data[5] +
			o._data[11] * _data[6] + _data[7];
		result._data[11] = o._data[3] * _data[8] + o._data[7] * _data[9] +
			o._data[11] * _data[10] + _data[11];

		return result;
	}


	float GetDeterminant() const {
		return _data[8] * _data[5] * _data[2] +
			_data[4] * _data[9] * _data[2] +
			_data[8] * _data[1] * _data[6] -
			_data[0] * _data[9] * _data[6] -
			_data[4] * _data[1] * _data[10] +
			_data[0] * _data[5] * _data[10];
	}

	void SetInverse(const Matrix4& m) {
		float det = GetDeterminant();
		if (det == 0) return;
		det = ((float)1.0f) / det;
		_data[0] = (-m._data[9] * m._data[6] + m._data[5] * m._data[10]) * det;
		_data[4] = (m._data[8] * m._data[6] - m._data[4] * m._data[10]) * det;
		_data[8] = (-m._data[8] * m._data[5] + m._data[4] * m._data[9] * m._data[15]) * det;
		_data[1] = (m._data[9] * m._data[2] - m._data[1] * m._data[10]) * det;
		_data[5] = (-m._data[8] * m._data[2] + m._data[0] * m._data[10]) * det;
		_data[9] = (m._data[8] * m._data[1] - m._data[0] * m._data[9] * m._data[15]) * det;
		_data[2] = (-m._data[5] * m._data[2] + m._data[1] * m._data[6] * m._data[15]) * det;
		_data[6] = (+m._data[4] * m._data[2] - m._data[0] * m._data[6] * m._data[15]) * det;
		_data[10] = (-m._data[4] * m._data[1] + m._data[0] * m._data[5] * m._data[15]) * det;
		_data[3] = (m._data[9] * m._data[6] * m._data[3]
			- m._data[5] * m._data[10] * m._data[3]
			- m._data[9] * m._data[2] * m._data[7]
			+ m._data[1] * m._data[10] * m._data[7]
			+ m._data[5] * m._data[2] * m._data[11]
			- m._data[1] * m._data[6] * m._data[11]) * det;
		_data[7] = (-m._data[8] * m._data[6] * m._data[3]
			+ m._data[4] * m._data[10] * m._data[3]
			+ m._data[8] * m._data[2] * m._data[7]
			- m._data[0] * m._data[10] * m._data[7]
			- m._data[4] * m._data[2] * m._data[11]
			+ m._data[0] * m._data[6] * m._data[11]) * det;
		_data[11] = (m._data[8] * m._data[5] * m._data[3]
			- m._data[4] * m._data[9] * m._data[3]
			- m._data[8] * m._data[1] * m._data[7]
			+ m._data[0] * m._data[9] * m._data[7]
			+ m._data[4] * m._data[1] * m._data[11]
			- m._data[0] * m._data[5] * m._data[11]) * det;
	}

	Matrix4 Inverse() const {
		Matrix4 result;
		result.SetInverse(*this);
		return result;
	}

	void Invert() {
		SetInverse(*this);
	}

	void SetOrientationAndPosition(const Quaternion& q, const Vector3& pos) {
		_data[0] = 1 - (2 * q._j * q._j + 2 * q._k * q._k);
		_data[1] = 2 * q._i * q._j + 2 * q._k * q._r;
		_data[2] = 2 * q._i * q._k - 2 * q._j * q._r;
		_data[3] = pos.x;
		_data[4] = 2 * q._i * q._j - 2 * q._k * q._r;
		_data[5] = 1 - (2 * q._i * q._i + 2 * q._k * q._k);
		_data[6] = 2 * q._j * q._k + 2 * q._i * q._r;
		_data[7] = pos.y;
		_data[8] = 2 * q._i * q._k + 2 * q._j * q._r;
		_data[9] = 2 * q._j * q._k - 2 * q._i * q._r;
		_data[10] = 1 - (2 * q._i * q._i + 2 * q._j * q._j);
		_data[11] = pos.z;
	}

	Vector3 localToWorld(const Vector3& local, const Matrix4& transform) {
		return transform.Transform(local);
	}

	Vector3 WorldToLocal(const Vector3& world, const Matrix4& transform) {
		return transform.TransformInverse(world);
	}

	Vector3 TransformInverse(const Vector3& vector) const {
		Vector3 tmp = vector;
		tmp.x -= _data[3];
		tmp.y -= _data[7];
		tmp.z -= _data[11];

		return Vector3(
			tmp.x * _data[0] + tmp.y * _data[4] + tmp.z * _data[8],
			tmp.x * _data[1] + tmp.y * _data[5] + tmp.z * _data[9],
			tmp.x * _data[2] + tmp.y * _data[6] + tmp.z * _data[10]
		);
	}

	Vector3 TransformDirection(const Vector3& vector) const {
		return Vector3(
			vector.x * _data[0] + vector.y * _data[1] + vector.z * _data[2],
			vector.x * _data[4] + vector.y * _data[5] + vector.z * _data[6],
			vector.x * _data[8] + vector.y * _data[9] + vector.z * _data[10]
		);
	}

	Vector3 TransformInverDirection(const Vector3& vector) const {
		return Vector3(
			vector.x * _data[0] + vector.y * _data[4] + vector.z * _data[8],
			vector.x * _data[1] + vector.y * _data[5] + vector.z * _data[9],
			vector.x * _data[2] + vector.y * _data[6] + vector.z * _data[10]
		);
	}

	Vector3 LocalToWorldDirection(const Vector3& local, const Matrix4& transform) {
		return transform.TransformDirection(local);
	}

	Vector3 WorldToLocalDirection(const Vector3& world, const Matrix4& transform) {
		return transform.TransformInverDirection(world);
	}

	void DirectXArray(float array[16]) const {
		array[0] = (float)_data[0];
		array[1] = (float)_data[4];
		array[2] = (float)_data[8];
		array[3] = (float)0;

		array[4] = (float)_data[1];
		array[5] = (float)_data[5];
		array[6] = (float)_data[9];
		array[7] = (float)0;

		array[8] = (float)_data[2];
		array[9] = (float)_data[6];
		array[10] = (float)_data[10];
		array[11] = (float)0;

		array[12] = (float)_data[3];
		array[13] = (float)_data[7];
		array[14] = (float)_data[11];
		array[15] = (float)1;
	}
};
