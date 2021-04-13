#pragma once
#include "CollisionPrimitives.h"


class IntersectionTests {
public:
	static bool SphereAndPlane(
		const Sphere& sphere,
		const CollisionPlane& plane);

	static bool SphereAndSphere(
		const Sphere& a,
		const Sphere& b);

	static bool BoxAndBox(
		const Box& a,
		const Box& b);

	static bool BoxAndPlane(
		const Box& box,
		const CollisionPlane& plane);
};
