#pragma once
#include "Vector3.h"
#include "Rigidbody.h"

struct BoundingSphere {
	//Holds the centre of the position for the Bounding Sphere
	Vector3 _centre;

	//Holds the radius of the object
	float _radius;

	//Basic constructor for creating a single bounding sphere
	BoundingSphere(const Vector3& centre, const float radius) {
		_centre = centre;
		_radius = radius;
	}

	//Constructor for creating a BoundingSphere that encompasses two bounding spheres
	BoundingSphere(const BoundingSphere& a, const BoundingSphere& b) {
		//Works out the offset for the center by getting the distance between the two spheres
		Vector3 centerOffset = b._centre - a._centre;

		//Gets the distance as a scalar value
		float distance = centerOffset.Magnitude();

		//Calculates the radius difference between the two spheres
		float radiusDifference = b._radius - a._radius;
		//if radius^2 >= distance
		if (radiusDifference * radiusDifference >= distance) {
			//if A is the larger sphere
			if (a._radius > b._radius) {
				//Set A as the bounding sphere
				_centre = a._centre;
				_radius = a._radius;
			}
			else
			{
				//If b is the larger sphere then set B as the bounding sphere
				_centre = b._centre;
				_radius = b._radius;
			}
		}
		//If the spheres are not in range of each other
		else
		{
			//calculate the new radius for the spheres
			distance = sqrtf(distance);

			//Calculate the radius
			_radius = (distance + a._radius + b._radius) * ((float)0.5);

			//Set the new center for the sphere
			_centre = a._centre;
			if (distance > 0) {
				//Change the center to include the offset calculate earlier
				_centre += centerOffset * ((_radius - a._radius) / distance);
			}
		}
	}

	bool IsOverlapping(const BoundingSphere* other) const {
		/*To check if there is a overlap between bounding spheres the formula is
		if the distance between the spheres squared is less than the radius of the two spheres squared
		then there is a collision
		*/
		float distanceSquared = (_centre - other->_centre).SquareMagnitude();
		return distanceSquared < (_radius + other->_radius) * (_radius + other->_radius);
	}

	float GetGrowth(const BoundingSphere& other) const {
		BoundingSphere newSpere(*this, other);

		return newSpere._radius * newSpere._radius - _radius * _radius;
	}

	float GetVolume() const {
		return ((float)1.333333) * 3.1459 * _radius * _radius * _radius;
	}

	struct PotentialContact
	{
		RigidbodyComponent* _bodies[2];
	};
};