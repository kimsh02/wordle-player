#pragma once

#include <string>
#include <unordered_set>

class RadixSorter {
    private:
	const std::unordered_set<std::string> &docs;

    public:
	RadixSorter(const std::unordered_set<std::string> &);
};
