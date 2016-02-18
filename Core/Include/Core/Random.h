#include <stdlib.h>
#include <time.h>

namespace sge
{
	void randomInitialize() // Initializes with "random" seed
	{
		srand(time(NULL));
	}

	void randomInitialize(unsigned int seed) // Initializes with given seed
	{
		srand(seed);
	}

	int random() // Generates random number
	{
		return rand();
	}

	int random(int min, int max) // Generates random int from range min-max
	{
		return min + rand() % max - min + 1;
	}

	template <typename T>
	T random(T min, T max) // Generates random float/double from range min-max
	{
		return min + (T(random()) / (T)RAND_MAX * (max - min));
	}
}