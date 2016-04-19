#pragma once

#include <SFML\System.hpp>
#include "Unit.hpp"


size_t RadiusFromMass(size_t mass);

template<typename T> inline T sqr(T n) { return n * n; }
template<typename T> inline T sign(T x){ return x > 0 ? 1 : x < 0 ? -1 : 0; }
template<typename T> inline T sgn(T x) { return x >= 0 ? 1 : -1; }
inline Unit::PositionType GetDistance_sqr(const Unit *unit1, const Unit *unit2)
{
	if(unit1 == nullptr || unit2 == nullptr)
		throw RunTimeException() << "Exception: FILE: " << __FILE__ << "; LINE = " << __LINE__;
	return sqr(unit1->GetPosition().x - unit2->GetPosition().x) + sqr(unit1->GetPosition().y - unit2->GetPosition().y);
}

inline size_t GetRandomNumber(size_t min, size_t max)
{
	if (min > max)
		throw RunTimeException() << "Exception: FILE: " <<__FILE__ << "; LINE = " << __LINE__;
	return (rand() % (max - min + 1)) + min;
}
