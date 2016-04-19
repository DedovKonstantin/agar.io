#ifndef IN_MUTEXABLE_HEADER
#error !IN_MUTEXABLE_HEADER
#endif


template<typename T>
Mutexable<T>::Mutexable()
{
}

template<typename T>
T& Mutexable<T>::Get()
{
	return data;
}

template<typename T>
void Mutexable<T>::Lock()
{
	mutex.lock();
}

template<typename T>
void Mutexable<T>::Unlock()
{
	mutex.unlock();
}

template<typename T>
void Mutexable<T>::Wait()
{
	mutex.lock();
	mutex.unlock();
}