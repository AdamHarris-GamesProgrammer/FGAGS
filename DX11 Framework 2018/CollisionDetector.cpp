#include "CollisionDetector.h"
#include "IntersectionTests.h"

static inline real TransfromToAxis(
	const Box& box, const Vector3& axis) {
	return
		box._halfSize.x * real_abs(axis * box.GetAxis(0)) +
		box._halfSize.y * real_abs(axis * box.GetAxis(1)) +
		box._halfSize.z * real_abs(axis * box.GetAxis(2));
}

unsigned CollisionDetector::SphereAndHalfSpace(const Sphere& sphere, const CollisionPlane& plane, CollisionData* data)
{
	if (data->_contactsLeft <= 0) return 0;

	Vector3 position = sphere.GetAxis(3);

	real ballDistance = plane._direction * position - sphere._radius - plane._offset;

	if (ballDistance >= 0) return 0;

	Contact* contact = data->_contacts;
	contact->_contactNormal = plane._direction;
	contact->_penetration = -ballDistance;
	contact->_contactPoint = position - plane._direction * (ballDistance + sphere._radius);
	contact->SetBodyData(sphere._body, NULL, data->_friction, data->_restitution);

	data->AddContacts(1);
	return 1;
}

unsigned CollisionDetector::SphereAndTruePlane(const Sphere& sphere, const CollisionPlane& plane, CollisionData* data)
{
	if (data->_contactsLeft <= 0) return 0;

	Vector3 position = sphere.GetAxis(3);

	real centreDisance = plane._direction * position - plane._offset;

	if (centreDisance * centreDisance > sphere._radius * sphere._radius) {
		return 0;
	}

	Vector3 normal = plane._direction;
	real penetration = -centreDisance;

	if (centreDisance < 0) {
		normal *= -1;
		penetration = -penetration;
	}

	penetration += sphere._radius;

	Contact* contact = data->_contacts;
	contact->_contactNormal = normal;
	contact->_penetration = penetration;
	contact->_contactPoint = position - plane._direction * centreDisance;
	contact->SetBodyData(sphere._body, NULL, data->_friction, data->_restitution);
	data->AddContacts(1);
	return 1;

}

unsigned CollisionDetector::SphereAndSphere(const Sphere& a, const Sphere& b, CollisionData* data)
{
	if (data->_contactsLeft <= 0) return 0;

	Vector3 positionA = a.GetAxis(3);
	Vector3 poistionB = b.GetAxis(3);

	Vector3 midLine = positionA - poistionB;
	real size = midLine.Magnitude();

	if (size <= 0.0f || size >= a._radius + b._radius) {
		return 0;
	}

	Vector3 normal = midLine * (((real)1.0) / size);

	Contact* contact = data->_contacts;
	contact->_contactNormal = normal;
	contact->_contactPoint = positionA + midLine * (real)0.5;
	contact->_penetration = (a._radius + b._radius - size);
	contact->SetBodyData(a._body, b._body, data->_friction, data->_restitution);

	data->AddContacts(1);
	return 1;

}

unsigned CollisionDetector::BoxAndHalfSpace(const Box& box, const CollisionPlane& plane, CollisionData* data)
{
	if (data->_contactsLeft <= 0) return 0;

	if (!IntersectionTests::BoxAndHalfSpace(box, plane)) return 0;

	//All combinations of vertices to check for contacts
	static real mults[8][3] = { {1,1,1},{-1,1,1},{1,-1,1},{-1,-1,1},
							   {1,1,-1},{-1,1,-1},{1,-1,-1},{-1,-1,-1} };

	Contact* contact = data->_contacts;
	unsigned contactsUsed = 0;
	for (unsigned i = 0; i < 8; i++) {
		Vector3 vertexPos(mults[i][0], mults[i][1], mults[i][2]);
		vertexPos.ComponentProductUpdate(box._halfSize);
		vertexPos = box.GetTransform().Transform(vertexPos);

		real vertexDistance = vertexPos * plane._direction;

		if (vertexDistance <= plane._offset) {
			contact->_contactPoint = plane._direction;
			contact->_contactPoint *= (vertexDistance - plane._offset);
			contact->_contactPoint += vertexPos;
			contact->_contactNormal = plane._direction;
			contact->_penetration = plane._offset - vertexDistance;

			contact->SetBodyData(box._body, NULL, data->_friction, data->_restitution);

			contact++;
			contactsUsed++;
			if (contactsUsed == (unsigned)data->_contactsLeft) return contactsUsed;
		}
	}

	data->AddContacts(contactsUsed);
	return contactsUsed;

}

static inline real PenetrationOnAxis(const Box& a, const Box& b, const Vector3& axis, const Vector3& centre) {
	real aProject = TransfromToAxis(a, axis);
	real bProject = TransfromToAxis(b, axis);

	real distance = real_abs(centre * axis);

	return aProject + bProject - distance;
}

static inline bool TryAxis(const Box& a, const Box& b, Vector3 axis, const Vector3& centre, unsigned index, real& smallestPenetration, unsigned& smallestCase) {
	if (axis.SquareMagnitude() < 0.0001) return true;
	axis.Normalize();

	real penetration = PenetrationOnAxis(a, b, axis, centre);

	if (penetration < 0)return false;
	if (penetration < smallestPenetration) {
		smallestPenetration = penetration;
		smallestCase = index;
	}
	return true;

}

void FillPointFaceBoxBox(const Box& a, const Box& b, const Vector3& centre, CollisionData* data, unsigned best, real pen) {
	Contact* contact = data->_contacts;

	Vector3 normal = a.GetAxis(best);
	if (a.GetAxis(best) * centre > 0) {
		normal = normal * -1.0f;
	}

	Vector3 vertex = b._halfSize;
	if (b.GetAxis(0) * normal < 0) vertex.x = -vertex.x;
	if (b.GetAxis(1) * normal < 0) vertex.y = -vertex.y;
	if (b.GetAxis(2) * normal < 0) vertex.z = -vertex.z;

	contact->_contactNormal = normal;
	contact->_penetration = pen;
	contact->_contactPoint = b.GetTransform() * vertex;
	contact->SetBodyData(a._body, b._body, data->_friction, data->_restitution);
}

static inline Vector3 ContactPoint(
	const Vector3& pOne, const Vector3& dOne, real oneSize, const Vector3& pTwo, const Vector3& dTwo, real twoSize, bool useOne) {
	Vector3 toSt, cOne, cTwo;
	real dpStaOne, dpStaTwo, dpOneTwo, smOne, smTwo;
	real denom, mua, mub;

	smOne = dOne.SquareMagnitude();
	smTwo = dTwo.SquareMagnitude();
	dpOneTwo = dTwo * dOne;

	toSt = pOne - pTwo;
	dpStaOne = dOne * toSt;
	dpStaTwo = dTwo * toSt;

	denom = smOne * smTwo - dpOneTwo * dpOneTwo;

	// Zero denominator indicates parrallel lines
	if (real_abs(denom) < 0.0001f) {
		return useOne ? pOne : pTwo;
	}

	mua = (dpOneTwo * dpStaTwo - smTwo * dpStaOne) / denom;
	mub = (smOne * dpStaTwo - dpOneTwo * dpStaOne) / denom;

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
		cOne = pOne + dOne * mua;
		cTwo = pTwo + dTwo * mub;

		return cOne * 0.5 + cTwo * 0.5;
	}
}


unsigned CollisionDetector::BoxAndBox(const Box& a, const Box& b, CollisionData* data)
{
	Vector3 toCentre = b.GetAxis(3) - a.GetAxis(3);

	real pen = REAL_MAX;
	unsigned best = 0xffffff;


	if (!TryAxis(a, b, a.GetAxis(0), toCentre, 0, pen, best)) return 0;
	if (!TryAxis(a, b, a.GetAxis(1), toCentre, 1, pen, best)) return 0;
	if (!TryAxis(a, b, a.GetAxis(2), toCentre, 2, pen, best)) return 0;

	if (!TryAxis(a, b, b.GetAxis(0), toCentre, 3, pen, best)) return 0;
	if (!TryAxis(a, b, b.GetAxis(1), toCentre, 4, pen, best)) return 0;
	if (!TryAxis(a, b, b.GetAxis(2), toCentre, 5, pen, best)) return 0;

	unsigned bestSingleAxis = best;

	if (!TryAxis(a, b, a.GetAxis(0) % b.GetAxis(0), toCentre, 6, pen, best))return 0;
	if (!TryAxis(a, b, a.GetAxis(0) % b.GetAxis(1), toCentre, 7, pen, best))return 0;
	if (!TryAxis(a, b, a.GetAxis(0) % b.GetAxis(2), toCentre, 8, pen, best))return 0;
	if (!TryAxis(a, b, a.GetAxis(1) % b.GetAxis(0), toCentre, 9, pen, best))return 0;
	if (!TryAxis(a, b, a.GetAxis(1) % b.GetAxis(1), toCentre, 10, pen, best))return 0;
	if (!TryAxis(a, b, a.GetAxis(1) % b.GetAxis(2), toCentre, 11, pen, best))return 0;
	if (!TryAxis(a, b, a.GetAxis(2) % b.GetAxis(0), toCentre, 12, pen, best))return 0;
	if (!TryAxis(a, b, a.GetAxis(2) % b.GetAxis(1), toCentre, 13, pen, best))return 0;
	if (!TryAxis(a, b, a.GetAxis(2) % b.GetAxis(2), toCentre, 14, pen, best))return 0;


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

unsigned CollisionDetector::BoxAndPoint(const Box& box, const Vector3& point, CollisionData* data)
{
	// Transform the point into box coordinates
	Vector3 relPt = box.GetTransform().TransformInverse(point);

	Vector3 normal;

	// Check each axis, looking for the axis on which the
	// penetration is least deep.
	real min_depth = box._halfSize.x - real_abs(relPt.x);
	if (min_depth < 0) return 0;
	normal = box.GetAxis(0) * ((relPt.x < 0) ? -1 : 1);

	real depth = box._halfSize.y - real_abs(relPt.y);
	if (depth < 0) return 0;
	else if (depth < min_depth)
	{
		min_depth = depth;
		normal = box.GetAxis(1) * ((relPt.y < 0) ? -1 : 1);
	}

	depth = box._halfSize.z - real_abs(relPt.z);
	if (depth < 0) return 0;
	else if (depth < min_depth)
	{
		min_depth = depth;
		normal = box.GetAxis(2) * ((relPt.z < 0) ? -1 : 1);
	}

	// Compile the contact
	Contact* contact = data->_contacts;
	contact->_contactNormal = normal;
	contact->_contactPoint = point;
	contact->_penetration = min_depth;

	// Note that we don't know what rigid body the point
	// belongs to, so we just use NULL. Where this is called
	// this value can be left, or filled in.
	contact->SetBodyData(box._body, NULL,
		data->_friction, data->_restitution);

	data->AddContacts(1);
	return 1;
}

unsigned CollisionDetector::BoxAndSphere(const Box& box, const Sphere& sphere, CollisionData* data)
{
	// Transform the centre of the sphere into box coordinates
	Vector3 centre = sphere.GetAxis(3);
	Vector3 relCentre = box.GetTransform().TransformInverse(centre);

	// Early out check to see if we can exclude the contact
	if (real_abs(relCentre.x) - sphere._radius > box._halfSize.x ||
		real_abs(relCentre.y) - sphere._radius > box._halfSize.y ||
		real_abs(relCentre.z) - sphere._radius > box._halfSize.z)
	{
		return 0;
	}

	Vector3 closestPt(0, 0, 0);
	real dist;

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
	contact->_penetration = sphere._radius - real_sqrt(dist);
	contact->SetBodyData(box._body, sphere._body,
		data->_friction, data->_restitution);

	data->AddContacts(1);
	return 1;
}



