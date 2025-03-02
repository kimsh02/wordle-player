#include <utility.hpp>

#include <iostream>

void printSet(const std::unordered_set<std::string> &set)
{
	std::cout << "\n[";
	for (const auto &word : set) {
		std::cout << word << ", ";
	}
	std::cout << "]";
}
