#ifndef MEMORY_H_
#define MEMORY_H_

template<class T> void SafeDelete(T& instance)
	{
		if (instance) {

			delete instance;
			instance = nullptr;
		}
	}

	template<class T> void SafeDeleteArray(T& instance)
	{
		if (instance) {

			delete[] instance;
			instance = nullptr;
		}
	}

	template<class T> void SafeRelease(T& instance)
	{
		if (instance) {

			instance->Release();
			instance = nullptr;
		}
	}

#endif