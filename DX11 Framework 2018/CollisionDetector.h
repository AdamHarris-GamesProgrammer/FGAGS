#pragma once
#include "CollisionPrimitives.h"
#include "CollisionData.h"
#include <vector>

class CollisionDetector
{
public:
	/// <summary>
	/// Static method for Detecting collisions between a sphere and a plane.
	/// </summary>
	/// <param name="sphere">The sphere we are testing</param>
	/// <param name="plane">The plane we are testing</param>
	/// <param name="data">The collision data array</param>
	/// <returns>Returns the number of contacts generated</returns>
	static unsigned SphereAndTruePlane(const Sphere& sphere,const CollisionPlane& plane,CollisionData* data);

	/// <summary>
	/// Static method for detecting collisions between two spheres
	/// </summary>
	/// <param name="a">The first sphere</param>
	/// <param name="b">The second sphere</param>
	/// <param name="data">The collision data array</param>
	/// <returns>Returns the number of contacts generated</returns>
	static unsigned SphereAndSphere(const Sphere& a,const Sphere& b,CollisionData* data);

	/// <summary>
	/// Static method for detecting collisions between a box and a plane
	/// </summary>
	/// <param name="box">The box we are testing</param>
	/// <param name="plane">The plane we are testing</param>
	/// <param name="data">The collision data array</param>
	/// <returns>Returns the number of contacts generated this frame</returns>
	static unsigned BoxAndPlane(const Box& box,const CollisionPlane& plane,CollisionData* data);

	/// <summary>
	/// Static method for detecting collisions between two box objects
	/// </summary>
	/// <param name="a">The first box</param>
	/// <param name="b">The second box</param>
	/// <param name="data">The collision data array</param>
	/// <returns>Returns the number of contacts generated this frame</returns>
	static unsigned BoxAndBox(const Box& a,const Box& b,CollisionData* data);

	/// <summary>
	/// Static method for detecting collisions between a box and a sphere object
	/// </summary>
	/// <param name="box">The box object we are testing</param>
	/// <param name="sphere">The sphere object we are testing</param>
	/// <param name="data">The collisions data array</param>
	/// <returns>Returns the number of contacts generated this frame</returns>
	static unsigned BoxAndSphere(const Box& box, const Sphere& sphere, CollisionData* data);
};



