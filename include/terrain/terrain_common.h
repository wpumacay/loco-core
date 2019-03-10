
#pragma once

// just in case
#define _USE_MATH_DEFINES
#include <cmath>

#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include <queue>

#include <tysoc_common.h>

#define _RANDOM( a, b ) ( a + ( b - a ) * ( std::rand() / ( float )RAND_MAX ) )
#define _RAND() ( std::rand() / ( float )RAND_MAX )
#define _RANDOM_COLOR { ( (float)rand() / RAND_MAX ), ( (float)rand() / RAND_MAX ), ( (float)rand() / RAND_MAX ) }