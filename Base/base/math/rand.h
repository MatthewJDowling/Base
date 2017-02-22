#pragma once

#include <random>
#include "flops.h"
#include <time.h>

namespace base
{
	
inline float rand01() 
{
	return (float)rand() / RAND_MAX; 
}

inline float randRange(float a, float b) { return flops::lerp(a, b, rand01()); }
}