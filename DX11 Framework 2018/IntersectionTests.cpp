#include "IntersectionTests.h"

static inline float TransfromToAxis(
	const Box& box, const Vector3& axis) {
	return
		box._halfSize.x * fabsf(axis * box.GetAxis(0)) +
		box._halfSize.y * fabsf(axis * box.GetAxis(1)) +
		box._halfSize.z * fabsf(axis * box.GetAxis(2));
}

bool IntersectionTests::SphereAndPlane(const Sphere& sphere, const CollisionPlane& plane)
{
	//gets the distance between the plane and the spheres position
	float ballDistance = plane._direction * sphere.GetAxis(3) - sphere._radius;

	//if the ball distance is less than the plane offset then they are intersecting
	return ballDistance <= plane._offset;
}

bool IntersectionTests::SphereAndSphere(const Sphere& a, const Sphere& b)
{
	//Gets a middle point between the spheres
	Vector3 midline = a.GetAxis(3) - b.GetAxis(3);

	//if the length of the middle line is less than the squared radii of the spheres than a collision has occured
	return midline.SquareMagnitude() < (a._radius + b._radius) * (a._radius + b._radius);
}


static inline bool OverlapOnAxis(const Box& a, const Box& b, const Vector3& axis, const Vector3& centre) {
	//Checks to see if two axis overlap
	float aProject = TransfromToAxis(a, axis);
	float bProject = TransfromToAxis(b, axis);

	float distance = fabsf(centre * axis);

	//if the distance between them is less than the projection of both then they intersect
	return(distance < aProject + bProject);
}

bool IntersectionTests::BoxAndBox(const Box& a, const Box& b)
{
	Vector3 toCentre = a.GetAxis(3) - a.GetAxis(3);

	//Check for a intersection on all the edges
	return (
		//Box A
		OverlapOnAxis(a, b, a.GetAxis(0), toCentre) &&
		OverlapOnAxis(a, b, a.GetAxis(1), toCentre) &&
		OverlapOnAxis(a, b, a.GetAxis(2), toCentre) &&
		//Box B
		OverlapOnAxis(a, b, b.GetAxis(0), toCentre) &&
		OverlapOnAxis(a, b, b.GetAxis(1), toCentre) &&
		OverlapOnAxis(a, b, b.GetAxis(2), toCentre) &&

		//Cross products
		OverlapOnAxis(a, b, a.GetAxis(0) % b.GetAxis(0), toCentre) &&
		OverlapOnAxis(a, b, a.GetAxis(0) % b.GetAxis(1), toCentre) &&
		OverlapOnAxis(a, b, a.GetAxis(0) % b.GetAxis(2), toCentre) &&
		OverlapOnAxis(a, b, a.GetAxis(1) % b.GetAxis(0), toCentre) &&
		OverlapOnAxis(a, b, a.GetAxis(1) % b.GetAxis(1), toCentre) &&
		OverlapOnAxis(a, b, a.GetAxis(1) % b.GetAxis(2), toCentre) &&
		OverlapOnAxis(a, b, a.GetAxis(2) % b.GetAxis(0), toCentre) &&
		OverlapOnAxis(a, b, a.GetAxis(2) % b.GetAxis(1), toCentre) &&
		OverlapOnAxis(a, b, a.GetAxis(2) % b.GetAxis(2), toCentre)
		);
}

bool IntersectionTests::BoxAndPlane(const Box& box, const CollisionPlane& plane)
{
	//Works out the radius of the box
	float projectedRadius = TransfromToAxis(box, plane._direction);

	//gets the distance between the box and the plane
	float boxDistance = plane._direction * box.GetAxis(3) - projectedRadius;

	//if the box distance is less than the plane offset then a intersection has happened
	return boxDistance <= plane._offset;
}