#include "Core/Random.h"

void randomInitialize() // Initializes with a "random" seed
{
	srand(time(NULL));
}

void randomInitialize(const unsigned int &seed) // Initializes with the given seed
{
	srand(seed);
}

int random() // Generates a random number
{
	return rand();
}

int random(const int &min, const int &max) // Generates a random integer from range min-max
{
	if (min < max)
	{
		return min + rand() % (max - min + 1);
	}
	return max + rand() % (min - max + 1);
}