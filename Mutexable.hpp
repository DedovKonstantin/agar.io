#pragma once

#include <SFML\System.hpp>

using namespace sf;

template<typename T>
class Mutexable
{
private:
	Mutex mutex;
	T data;
public:
	Mutexable();
	T& Get();
	void Lock();
	void Unlock();
	void Wait();

};
#define IN_MUTEXABLE_HEADER
#include "MutexableRealisation.h"
#undef IN_MUTEXABLE_HEADER
