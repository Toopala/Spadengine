#include "Core/Math.h"
#include "Core/Testi.h"

#include <iostream>

int main(int argc, char** argv)
{
	//testing branch
	sge::math::vec2 vec(2.0f);
	sge::printShit("TESTI");

	std::cout << vec.x << ", " << vec.y << std::endl;

	return 0;
}