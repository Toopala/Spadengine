#include <stdlib.h>
#include <time.h>

namespace sge
{
	void randomInitialize();
	void randomInitialize(const unsigned int &seed);
	int random();
	int random(const int &min, const int &max);

	template <typename T>
	T random(const T &min, const T &max) // Generates a random float/double from range min-max
	{
		return min + (T(random()) / (T)RAND_MAX * (max - min));
	}
}