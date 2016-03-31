#include "Core/Random.h"

namespace sge
{
	void randomInitialize()
	{
		srand(time(NULL));
	}

	void randomInitialize(const unsigned int &seed)
	{
		srand(seed);
	}

	int random(const int &min, const int &max)
	{
		if (min < max)
		{
			return min + rand() % (max - min + 1);
		}
		return max + rand() % (min - max + 1);
	}
}