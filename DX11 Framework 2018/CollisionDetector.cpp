#include "CollisionDetector.h"
#include "IntersectionTests.h"

/// <summary>
/// Projects the half-size of a box onto a axis
/// </summary>
/// <returns>Returns the total size of the axis</returns>
static inline float TransfromToAxis(const Box& box, const Vector3& axis) {
	return
		box._halfSize.x * fabsf(axis * box.GetAxis(0)) +
		box._halfSize.y * fabsf(axis * box.GetAxis(1)) +
		box._halfSize.z * fabsf(axis * box.GetAxis(2));
}


unsigned CollisionDetector::SphereAndTruePlane(const Sphere& sphere, const CollisionPlane& plane, CollisionData* data)
{
	//Checks if we have contacts left this frame
	if (data->_contactsLeft <= 0) return 0;

	//Check if the sphere intersects the plane
	if (!IntersectionTests::SphereAndPlane(sphere, plane)) return 0;
	//A collision has occured

	//Get our spheres position
	Vector3 position = sphere.GetAxis(3);
	//Calculate the distance from the plane to the center of the sphere
	float distanceFromCenter = plane._direction * position - plane._offset;

	//Get the planes direction for the normal vector
	Vector3 normal = plane._direction;
	//Invert the distance from center to calculate our interpenetration
	float interpenetration = -distanceFromCenter;

	//Check which side of the plane we are on
	if (distanceFromCenter < 0) {
		normal *= -1;
		interpenetration = -interpenetration;
	}

	//Adds the spheres radius to our penetration
	interpenetration += sphere._radius;

	//Creates a new contact with our calculated information 
	Contact* contact = data->_contacts;
	contact->_contactNormal = normal;
	contact->_penetration = interpenetration;
	//Calculate our point of contact
	contact->_contactPoint = position - plane._direction * distanceFromCenter;

	//Set the data for our collision
	contact->SetBodyData(sphere._body, NULL, data->_friction, data->_restitution);
	data->AddContacts(1);
	return 1;

}

unsigned CollisionDetector::SphereAndSphere(const Sphere& a, const Sphere& b, CollisionData* data)
{
	//Check we have contacts remaining
	if (data->_contactsLeft <= 0) return 0;
	
	//See if the spheres have intersected each other
	if (!IntersectionTests::SphereAndSphere(a, b)) return 0;

	//get the position of the two spheres
	Vector3 positionA = a.GetAxis(3);
	Vector3 poistionB = b.GetAxis(3);

	//Find the vector between the objects
	Vector3 midLine = positionA - poistionB;
	float size = midLine.Magnitude();

	
	//Calculate our collision normal
	Vector3 normal = midLine * (1.0f / size);

	//Generate a contact
	Contact* contact = data->_contacts;
	contact->_contactNormal = normal;
	contact->_contactPoint = positionA + midLine * 0.5f;
	contact->_penetration = (a._radius + b._radius - size);
	contact->SetBodyData(a._body, b._body, data->_friction, data->_restitution);

	data->AddContacts(1);
	return 1;
}

unsigned CollisionDetector::BoxAndPlane(const Box& box, const CollisionPlane& plane, CollisionData* data)
{
	//Check we have collisions available.
	if (data->_contactsLeft <= 0) return 0;

	//Perform an intersection test to see if we have intersected
	if (!IntersectionTests::BoxAndPlane(box, plane)) return 0;

	//All combinations of vertices to check for contacts
	static float mults[8][3] = { {1,1,1},{-1,1,1},{1,-1,1},{-1,-1,1}, {1,1,-1},{-1,1,-1},{1,-1,-1},{-1,-1,-1} };

	//Generate a contact
	Contact* contact = data->_contacts;
	//store the contacts we have used
	unsigned contactsUsed = 0;

	//Cycle through all vertices to generate all needed contacts
	for (unsigned i = 0; i < 8; i++) {
		Vector3 vertexPos(mults[i][0], mults[i][1], mults[i][2]);

		vertexPos.ComponentProductUpdate(box._halfSize);
		vertexPos = box.GetTransform().Transform(vertexPos);

		//Calculate the distance between the vertex and the plane
		float vertexDistance = vertexPos * plane._direction;

		//if the distance is less than the offset then we have a contact
		if (vertexDistance <= plane._offset) {
			contact->_contactPoint = plane._direction;
			contact->_contactPoint *= (vertexDistance - plane._offset);
			contact->_contactPoint += vertexPos;
			contact->_contactNormal = plane._direction;
			contact->_penetration = plane._offset - vertexDistance;

			contact->SetBodyData(box._body, NULL, data->_friction, data->_restitution);

			contact++;
			contactsUsed++;

			//Stops us from overflowing the contacts array
			if (contactsUsed == (unsigned)data->_contactsLeft) return contactsUsed;
		}
	}
	//Adds the amount of contacts we have used to the contacts system
	data->AddContacts(contactsUsed);
	return contactsUsed;

}

static inline bool TryAxis(const Box& a, const Box& b, Vector3 axis, const Vector3& centre, unsigned index, float& smallestPenetration, unsigned& smallestCase) {
	if (axis.SquareMagnitude() < 0.0001) return true;
	axis.Normalize();

	//Checks if we have penetrated
	float aProject = TransfromToAxis(a, axis);
	float bProject = TransfromToAxis(b, axis);
	float distance = fabsf(centre * axis);
	float penetration = aProject + bProject - distance;

	//if we have not penetrated then return false
	if (penetration < 0)return false;

	//if we have penetrated see if this is the smallest penetration
	if (penetration < smallestPenetration) {
		smallestPenetration = penetration;
		smallestCase = index;
	}
	return true;
}

void FillPointFaceBoxBox(const Box& a, const Box& b, const Vector3& centre, CollisionData* data, unsigned best, float pen) {
	//Get this contact
	Contact* contact = data->_contacts;

	//best is the axis with the collision, this now works out which faces the collision is between
	Vector3 normal = a.GetAxis(best);
	if (a.GetAxis(best) * centre > 0) {
		normal = normal * -1.0f;
	}

	//work out which vertex of the boxes we colliding with
	Vector3 vertex = b._halfSize;
	if (b.GetAxis(0) * normal < 0) vertex.x = -vertex.x;
	if (b.GetAxis(1) * normal < 0) vertex.y = -vertex.y;
	if (b.GetAxis(2) * normal < 0) vertex.z = -vertex.z;

	//Create the contact data
	contact->_contactNormal = normal;
	contact->_penetration = pen;
	contact->_contactPoint = b.GetTransform() * vertex;
	contact->SetBodyData(a._body, b._body, data->_friction, data->_restitution);
}

static inline Vector3 ContactPoint(
	const Vector3& pOne, const Vector3& dOne, float oneSize, const Vector3& pTwo, const Vector3& dTwo, float twoSize, bool useOne) {

	float smOne = dOne.SquareMagnitude();
	float smTwo = dTwo.SquareMagnitude();
	float dpOneTwo = dTwo * dOne;

	Vector3 toSt = pOne - pTwo;
	float dpStaOne = dOne * toSt;
	float dpStaTwo = dTwo * toSt;

	float denominator = smOne * smTwo - dpOneTwo * dpOneTwo;

	// Zero denominator indicates parallel lines
	if (fabsf(denominator) < 0.0001f) {
		return useOne ? pOne : pTwo;
	}

	float mua = (dpOneTwo * dpStaTwo - smTwo * dpStaOne) / denominator;
	float mub = (smOne * dpStaTwo - dpOneTwo * dpStaOne) / denominator;

	// If either of the edges has the nearest point out
	// of bounds, then the edges aren't crossed, we have
	// an edge-face contact. Our point is on the edge, which
	// we know from the useOne parameter.
	if (mua > oneSize ||
		mua < -oneSize ||
		mub > twoSize ||
		mub < -twoSize)
	{
		return useOne ? pOne : pTwo;
	}
	else
	{
		Vector3 cOne = pOne + dOne * mua;
		Vector3 cTwo = pTwo + dTwo * mub;

		return cOne * 0.5 + cTwo * 0.5;
	}
}

//Macro for the TryAxis function, simplifies the code
#define OVERLAPPING(axis, index) \
	if(!TryAxis(a, b, (axis), toCentre, (index), pen, best)) return 0;

unsigned CollisionDetector::BoxAndBox(const Box& a, const Box& b, CollisionData* data)
{
	Vector3 toCentre = b.GetAxis(3) - a.GetAxis(3);

	float pen = FLT_MAX;
	unsigned best = 0xffffff;


	OVERLAPPING(a.GetAxis(0), 0);
	OVERLAPPING(a.GetAxis(1), 1);
	OVERLAPPING(a.GetAxis(2), 2);

	OVERLAPPING(b.GetAxis(0), 3);
	OVERLAPPING(b.GetAxis(1), 4);
	OVERLAPPING(b.GetAxis(2), 5);

	unsigned bestSingleAxis = best;

	OVERLAPPING(a.GetAxis(0) % b.GetAxis(0), 6);
	OVERLAPPING(a.GetAxis(0) % b.GetAxis(1), 7);
	OVERLAPPING(a.GetAxis(0) % b.GetAxis(2), 8);
	OVERLAPPING(a.GetAxis(1) % b.GetAxis(0), 9);
	OVERLAPPING(a.GetAxis(1) % b.GetAxis(1), 10);
	OVERLAPPING(a.GetAxis(1) % b.GetAxis(2), 11);
	OVERLAPPING(a.GetAxis(2) % b.GetAxis(0), 12);
	OVERLAPPING(a.GetAxis(2) % b.GetAxis(1), 13);
	OVERLAPPING(a.GetAxis(2) % b.GetAxis(2), 14);


	assert(best != 0xffffff);

	if (best < 3) {
		FillPointFaceBoxBox(a, b, toCentre, data, best, pen);
		data->AddContacts(1);
		return 1;
	}
	else if (best < 6) {
		FillPointFaceBoxBox(a, b, toCentre * -1.0f, data, best - 3, pen);
		data->AddContacts(1);
		return 1;
	}
	else
	{
		best -= 6;
		unsigned aAxisIndex = best / 3;
		unsigned bAxisIndex = best % 3;
		Vector3 aAxis = a.GetAxis(aAxisIndex);
		Vector3 bAxis = b.GetAxis(bAxisIndex);
		Vector3 axis = aAxis % bAxis;
		axis.Normalize();

		if (axis * toCentre > 0) axis = axis * -1.0f;

		Vector3 ptOnOneEdge = a._halfSize;
		Vector3 ptOnTwoEdge = b._halfSize;
		for (unsigned i = 0; i < 3; i++)
		{
			if (i == aAxisIndex) ptOnOneEdge[i] = 0;
			else if (a.GetAxis(i) * axis > 0) ptOnOneEdge[i] = -ptOnOneEdge[i];

			if (i == bAxisIndex) ptOnTwoEdge[i] = 0;
			else if (b.GetAxis(i) * axis < 0) ptOnTwoEdge[i] = -ptOnTwoEdge[i];
		}

		// Move them into world coordinates (they are already oriented
		// correctly, since they have been derived from the axes).
		ptOnOneEdge = a.GetTransform() * ptOnOneEdge;
		ptOnTwoEdge = b.GetTransform() * ptOnTwoEdge;

		// So we have a point and a direction for the colliding edges.
		// We need to find out point of closest approach of the two
		// line-segments.
		Vector3 vertex = ContactPoint(
			ptOnOneEdge, aAxis, a._halfSize[aAxisIndex],
			ptOnTwoEdge, bAxis, b._halfSize[bAxisIndex],
			bestSingleAxis > 2
		);

		// We can fill the contact.
		Contact* contact = data->_contacts;

		contact->_penetration = pen;
		contact->_contactNormal = axis;
		contact->_contactPoint = vertex;
		contact->SetBodyData(a._body, b._body,
			data->_friction, data->_restitution);
		data->AddContacts(1);
		return 1;
	}
	return 0;
}

unsigned CollisionDetector::BoxAndSphere(const Box& box, const Sphere& sphere, CollisionData* data)
{
	// Transform the centre of the sphere into box coordinates
	Vector3 centre = sphere.GetAxis(3);
	Vector3 relCentre = box.GetTransform().TransformInverse(centre);

	// Early out check to see if we can exclude the contact
	if (fabsf(relCentre.x) - sphere._radius > box._halfSize.x ||
		fabsf(relCentre.y) - sphere._radius > box._halfSize.y ||
		fabsf(relCentre.z) - sphere._radius > box._halfSize.z)
	{
		return 0;
	}

	Vector3 closestPt(0, 0, 0);
	float dist;

	// Clamp each coordinate to the box.
	dist = relCentre.x;
	if (dist > box._halfSize.x) dist = box._halfSize.x;
	if (dist < -box._halfSize.x) dist = -box._halfSize.x;
	closestPt.x = dist;

	dist = relCentre.y;
	if (dist > box._halfSize.y) dist = box._halfSize.y;
	if (dist < -box._halfSize.y) dist = -box._halfSize.y;
	closestPt.y = dist;

	dist = relCentre.z;
	if (dist > box._halfSize.z) dist = box._halfSize.z;
	if (dist < -box._halfSize.z) dist = -box._halfSize.z;
	closestPt.z = dist;

	// Check we're in contact
	dist = (closestPt - relCentre).SquareMagnitude();
	if (dist > sphere._radius * sphere._radius) return 0;

	// Compile the contact
	Vector3 closestPtWorld = box.GetTransform().Transform(closestPt);

	Contact* contact = data->_contacts;
	contact->_contactNormal = (closestPtWorld - centre);
	contact->_contactNormal.Normalize();
	contact->_contactPoint = closestPtWorld;
	contact->_penetration = sphere._radius - sqrtf(dist);
	contact->SetBodyData(box._body, sphere._body,
		data->_friction, data->_restitution);

	data->AddContacts(1);
	return 1;
}