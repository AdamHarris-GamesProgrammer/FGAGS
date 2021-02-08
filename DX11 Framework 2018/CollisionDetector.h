#pragma once
#include "CollisionPrimitives.h"
#include "CollisionData.h"

class CollisionDetector
{
public:
	unsigned SphereAndHalfSpace(
		const Sphere& sphere,
		const Plane& plane,
		CollisionData* data);

	unsigned SphereAndTruePlane(
		const Sphere& sphere,
		const Plane& plane,
		CollisionData* data);

	unsigned SphereAndSphere(
		const Sphere& a,
		const Sphere& b,
		CollisionData* data);

	unsigned BoxAndHalfSpace(
		const Box& box,
		const Plane& plane,
		CollisionData* data);

	unsigned BoxAndBox(
		const Box& a,
		const Box& b,
		CollisionData* data);

	unsigned BoxAndPoint(
		const Box& box,
		const Vector3& point,
		CollisionData* data);

	unsigned BoxAndSphere(
		const Box& box,
		const Sphere& sphere,
		CollisionData* data);


};

class IntersectionTests {
public:
	static bool SphereAndHalfSpace(
		const Sphere& sphere,
		const Plane& plane);

	static bool SphereAndSphere(
		const Sphere& a,
		const Sphere& b);

	static bool BoxAndBox(
		const Box& a,
		const Box& b);

	static bool BoxAndHalfSpace(
		const Box& box,
		const Plane& plane);
};

