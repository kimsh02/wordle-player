#include "index.hpp"

#include <iostream>

Index::Index(char l, size_t p)
	: letter{ l }
	, pos{ p }
{
}

bool Index::operator==(const Index &other) const
{
	return (letter == other.getLetter() && pos == other.getPos());
}

void Index::printIndex(void) const
{
	std::cout << letter << pos << "\n";
}

char Index::getLetter(void) const
{
	return letter;
}

size_t Index::getPos(void) const
{
	return pos;
}

size_t Index::getCount(void) const
{
	return count;
}
