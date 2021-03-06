#pragma once
#include "Vector3.h"
#include "Quaternion.h"

//A 3x3 Matrix
class Matrix3 {
public:
	//Holds the matrix data in array form
	float _data[9];

	/// <summary>
	/// Sets all of the array to 0
	/// </summary>
	Matrix3() {
		_data[0] = _data[1] = _data[2] = _data[3] = _data[4] = _data[5] = _data[6] = _data[7] = _data[8] = 0;
	}

	//Sets the data to 9 given values
	Matrix3(float e0, float e1, float e2, float e3, float e4, float e5, float e6, float e7, float e8) {
		_data[0] = e0;
		_data[1] = e1;
		_data[2] = e2;
		_data[3] = e3;
		_data[4] = e4;
		_data[5] = e5;
		_data[6] = e6;
		_data[7] = e7;
		_data[8] = e8;
	}

	/// <summary>
	/// Sets the value of the matrix as an inertia tensor for a cuboid based on it's size and mass. 
	/// </summary>
	/// <param name="halfSizes">The extents of the cuboid. A 2m cube would have a half size of (1,1,1)</param>
	/// <param name="mass">The mass of the object</param>
	void SetBlockInertiaTensor(const Vector3& halfSizes, float mass) {
		Vector3 squares = halfSizes.ComponentProduct(halfSizes);
		SetInertiaTensorCoeffs(0.3f * mass * (squares.y + squares.z), 0.3f * mass * (squares.x + squares.z),
			0.3f * mass * (squares.x + squares.y));
	}

	//Sets the values of the matrix based on a inertia tensor
	void SetInertiaTensorCoeffs(float ix, float iy, float iz,
		float ixy = 0, float ixz = 0, float iyz = 0)
	{
		_data[0] = ix;
		_data[1] = _data[3] = -ixy;
		_data[2] = _data[6] = -ixz;
		_data[4] = iy;
		_data[5] = _data[7] = -iyz;
		_data[8] = iz;
	}

	//Sets the matrix values based on 3 vectors
	void SetComponents(const Vector3& a, const Vector3& b, const Vector3& c) {
		_data[0] = a.x;
		_data[1] = b.x;
		_data[2] = c.x;
		_data[3] = a.y;
		_data[4] = b.y;
		_data[5] = c.y;
		_data[6] = a.z;
		_data[7] = b.z;
		_data[8] = c.z;
	}

	//Sets the matrix to be a skew symmetric based on the vector passed in
	void SetSkewSymmetric(const Vector3 vector) {
		_data[0] = _data[4] = _data[8] = 0;
		_data[1] = -vector.z;
		_data[2] = vector.y;
		_data[3] = vector.z;
		_data[5] = -vector.x;
		_data[6] = -vector.y;
		_data[7] = vector.x;
	}

	//Adds a matrix to this matrix
	void operator+=(const Matrix3& o)
	{
		_data[0] += o._data[0];
		_data[1] += o._data[1];
		_data[2] += o._data[2];
		_data[3] += o._data[3];
		_data[4] += o._data[4];
		_data[5] += o._data[5];
		_data[6] += o._data[6];
		_data[7] += o._data[7]; 
		_data[8] += o._data[8];
	}

	//Multiplies the values in this matrix by a scalar
	void operator*=(const float scalar) {
		_data[0] *= scalar;
		_data[1] *= scalar;
		_data[2] *= scalar;
		_data[3] *= scalar;
		_data[4] *= scalar;
		_data[5] *= scalar;
		_data[6] *= scalar;
		_data[7] *= scalar;
		_data[8] *= scalar;
	}

	//Transforms a provided vector by this matrix
	Vector3 operator*(const Vector3& vector) const {
		return Vector3(
			vector.x * _data[0] + vector.y * _data[1] + vector.z * _data[2],
			vector.x * _data[3] + vector.y * _data[4] + vector.z * _data[5],
			vector.x * _data[6] + vector.y * _data[7] + vector.z * _data[8]
		);
	}

	//Transform a provided vector by this matrix
	Vector3 Transform(const Vector3& vector) const {
		return (*this) * vector;
	}

	//Transforms the given vector by the transpose of this matrix
	Vector3 TransformTranspose(const Vector3& vector) const {
		return Vector3(
			vector.x * _data[0] + vector.y * _data[3] + vector.z * _data[6],
			vector.x * _data[1] + vector.y * _data[4] + vector.z * _data[7],
			vector.x * _data[2] + vector.y * _data[5] + vector.z * _data[8]
		);
	}

	//Multiplies  this matrix by another matrix
	Matrix3 operator*(const Matrix3& o) const {
		return Matrix3(
			_data[0] * o._data[0] + _data[1] * o._data[3] + _data[2] * o._data[6],
			_data[0] * o._data[1] + _data[1] * o._data[4] + _data[2] * o._data[7],
			_data[0] * o._data[2] + _data[1] * o._data[5] + _data[2] * o._data[8],
			_data[3] * o._data[0] + _data[4] * o._data[3] + _data[5] * o._data[6],
			_data[3] * o._data[1] + _data[4] * o._data[4] + _data[5] * o._data[7],
			_data[3] * o._data[2] + _data[4] * o._data[5] + _data[5] * o._data[8],
			_data[6] * o._data[0] + _data[7] * o._data[3] + _data[8] * o._data[6],
			_data[6] * o._data[1] + _data[7] * o._data[4] + _data[8] * o._data[7],
			_data[6] * o._data[2] + _data[7] * o._data[5] + _data[8] * o._data[8]
		);
	}

	//Multiplies this matrix by another matrix
	void operator*=(const Matrix3& o) {
		float t1;
		float t2;
		float t3;
		t1 = _data[0] * o._data[0] + _data[1] * o._data[3] + _data[2] * o._data[6];
		t2 = _data[0] * o._data[1] + _data[1] * o._data[4] + _data[2] * o._data[7];
		t3 = _data[0] * o._data[2] + _data[1] * o._data[5] + _data[2] * o._data[8];
		_data[0] = t1;
		_data[1] = t2;
		_data[2] = t3;
		t1 = _data[3] * o._data[0] + _data[4] * o._data[3] + _data[5] * o._data[6];
		t2 = _data[3] * o._data[1] + _data[4] * o._data[4] + _data[5] * o._data[7];
		t3 = _data[3] * o._data[2] + _data[4] * o._data[5] + _data[5] * o._data[8];
		_data[3] = t1;
		_data[4] = t2;
		_data[5] = t3;
		t1 = _data[6] * o._data[0] + _data[7] * o._data[3] + _data[8] * o._data[6];
		t2 = _data[6] * o._data[1] + _data[7] * o._data[4] + _data[8] * o._data[7];
		t3 = _data[6] * o._data[2] + _data[7] * o._data[5] + _data[8] * o._data[8];
		_data[6] = t1;
		_data[7] = t2;
		_data[8] = t3;
	}

	//Sets this matrix to be the inverse of another matrix
	void SetInverse(const Matrix3& m) {
		float e1 = m._data[0] * m._data[4];
		float e2 = m._data[0] * m._data[5];
		float e3 = m._data[1] * m._data[3];
		float e4 = m._data[2] * m._data[3];
		float e5 = m._data[1] * m._data[6];
		float e6 = m._data[2] * m._data[6];

		//Get the determinant
		float det = (e1 * m._data[8] - e2 * m._data[7] - e3 * m._data[8] + 
					e4 * m._data[7] + e5 * m._data[5] - e6 * m._data[4]);

		if (det == 0.0f) return;
		float inverseDet = 1.0f / det;

		_data[0] = (m._data[4] * m._data[8] - m._data[5] * m._data[7]) * inverseDet;
		_data[1] = -(m._data[1] * m._data[8] - m._data[2] * m._data[7]) * inverseDet;
		_data[2] = (m._data[1] * m._data[5] - m._data[2] * m._data[4]) * inverseDet;
		_data[3] = -(m._data[3] * m._data[8] - m._data[5] * m._data[6]) * inverseDet;
		_data[4] = (m._data[0] * m._data[8] - e6) * inverseDet;
		_data[5] = -(e2 - e4) * inverseDet;
		_data[6] = (m._data[3] * m._data[7] - m._data[4] * m._data[6]) * inverseDet;
		_data[7] = -(m._data[0] * m._data[7] - e5) * inverseDet;
		_data[8] = (e1 - e3) * inverseDet;
	}

	//Gets the inverse of this matrix
	Matrix3 Inverse() const {
		Matrix3 result;
		result.SetInverse(*this);
		return result;
	}

	//Inverts this matrix
	void Invert() {
		SetInverse(*this);
	}

	//Sets this matrix to another one
	void SetTranspose(const Matrix3& m) {
		_data[0] = m._data[0];
		_data[1] = m._data[3];
		_data[2] = m._data[6];
		_data[3] = m._data[1];
		_data[4] = m._data[4];
		_data[5] = m._data[7];
		_data[6] = m._data[2];
		_data[7] = m._data[5];
		_data[8] = m._data[8];
	}

	//Returns a transpose of this matrix
	Matrix3 Transpose() const {
		Matrix3 result;
		result.SetTranspose(*this);
		return result;
	}

	//Calculates this matrix in relation to a quaternion
	void SetOrientation(const Quaternion& q) {
		_data[0] = 1 - (2 * q._j * q._j + 2 * q._k * q._k);
		_data[1] = 2 * q._i * q._j + 2 * q._k * q._r;
		_data[2] = 2 * q._i * q._k - 2 * q._j * q._r;
		_data[3] = 2 * q._i * q._j - 2 * q._k * q._r;
		_data[4] = 1 - (2 * q._i * q._i + 2 * q._k * q._k);
		_data[5] = 2 * q._j * q._k + 2 * q._i * q._r;
		_data[6] = 2 * q._i * q._k + 2 * q._j * q._r;
		_data[7] = 2 * q._j * q._k - 2 * q._i * q._r;
		_data[8] = 1 - (2 * q._i * q._i + 2 * q._j * q._j);
	}
};
