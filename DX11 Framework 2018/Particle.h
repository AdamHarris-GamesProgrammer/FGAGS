#pragma once

#include "Vector3.h"

class Particle
{
protected:
	//Position in world space
	Vector3 _position;

	//Linear velocity in world space
	Vector3 _velocity;

	//Holds the acceleration of the particle.
	Vector3 _acceleration;

};

