#pragma once

#include <math.h>
#include <float.h>

#if 0 

#define SINGLE_PRECISION

typedef float real;

#define REAL_MAX FLT_MAX
#define real_sqrt sqrtf
#define real_abs fabsf
#define real_sin sinf
#define real_cos cosf
#define real_exp expf
#define real_pow powf
#define real_fmod fmodf
#define real_epsilon FLT_EPSILON
#define R_PI 3.14159

#else 

#define DOUBLE_PRECISION

//typedef double real;

#define REAL_MAX DBL_MAX
#define real_sqrt sqrt
#define real_abs fabs
#define real_sin sin
#define real_cos cos
#define real_exp exp
#define real_pow pow
#define real_fmod fmod
#define real_epsilon DBL_EPSILON
#define R_PI 3.1415926538979

#endif