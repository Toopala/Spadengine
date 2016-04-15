#include <stdlib.h>
#include <time.h>

namespace sge
{
	/** \brief Initializes with a random seed.
	*
	*	An initialization which uses time(NULL) as seed to generate random number sequence.
	*/
	void randomSeed();

	/** \brief Initializes with given seed.
	*
	*	An initialization which uses the given seed to generate a number sequence.
	*	\param const unsigned int &seed : The seed number
	*/
	void setSeed(const unsigned int &seed);

	/** \brief Generates a random integer from given range.
	*
	*	Gives the number from range even if minimum and maximum numbers are switched around.
	*	
	*	\param const int &min : Minimum number from range
	*	\param const int &max : Maximum number from range
	*	\return Returns the random integer.
	*/
	int random(const int &min, const int &max);

	/** \brief Generates a random number from given range.
	*
	*	A template function used for randomizing float and double numbers.
	*	Gives the number from range even if minimum and maximum numbers are switched around.
	*
	*	\param const T &min : Minimum number from range
	*	\param const T &max : Maximum number from range
	*	\return Returns the random float/double.
	*/
	template <typename T>
	T random(const T &min, const T &max)
	{
		return min + (T(rand()) / (T)RAND_MAX * (max - min));
	}
}
