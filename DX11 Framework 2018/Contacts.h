#pragma once
#include "Vector3.h"
#include "Precision.h"

class Contact {
public:

private:
	Vector3 _contactPoint;
	Vector3 _contactNormal;
	real _penetration;
};