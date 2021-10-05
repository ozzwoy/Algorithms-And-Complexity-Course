#pragma once
#include <random>

//Генератор випадкового беззнакового цілого в проміжку [min, max]
unsigned get_random_uint(unsigned min, unsigned max) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min, max);
	return dis(gen);
}
