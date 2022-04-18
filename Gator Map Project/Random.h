#pragma once
#include <random>

// Random class used in COP3503 project 3
class Random {
private:
	static std::mt19937 random;

public:
	static int RandomInt(int min, int max);
};
