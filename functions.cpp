#define _USE_MATH_DEFINES
#include "functions.hpp"
#include <cmath>

size_t RadiusFromMass(size_t mass)
{
	//pi * radius^2 * density = mass => radius = sqrt(mass / (density * pi))
	static const long double density = 0.01L;

	size_t result = (size_t)sqrt(mass / (density * M_PI));

	if (result == 0)
		++result;
	return result;
}