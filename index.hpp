#pragma once

#include <cstddef>
#include <functional>

struct Index {
    private:
	const char	  letter;
	const std::size_t pos;

	std::size_t count;

    public:
	Index(char, std::size_t);

	bool operator==(const Index &) const;

	void printIndex(void) const;

	char	    getLetter(void) const;
	std::size_t getPos(void) const;
	std::size_t getCount(void) const;
};

namespace std
{
template <>
struct hash<Index> {
	std::size_t operator()(const Index &other) const
	{
	}
};
}
