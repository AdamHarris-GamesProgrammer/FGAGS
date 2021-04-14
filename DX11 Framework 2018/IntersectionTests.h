#pragma once
#include "CollisionPrimitives.h"


class IntersectionTests {
public:
	//Checks for a intersection between a sphere and a plane 
	static bool SphereAndPlane(const Sphere& sphere, const CollisionPlane& plane);

	//Checks for a intersection between two spheres
	static bool SphereAndSphere(const Sphere& a, const Sphere& b);

	//Checks for a intersection between two boxes
	static bool BoxAndBox(const Box& a, const Box& b);

	//Checks for a intersection between a box and a plane 
	static bool BoxAndPlane(const Box& box, const CollisionPlane& plane);
};
