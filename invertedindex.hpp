#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class InvertedIndex {
    private:
	static constexpr std::size_t DOC_LENGTH	 = 5;
	static constexpr std::size_t MAX_INDICES = 130;

	std::unordered_map<std::string, std::vector<std::string> > invertedIndex;

	std::unordered_set<std::string> indices;

	const std::vector<std::string> &docs;

	void makeIndices(void);
	void printIndices(void) const;

	void makeInvertedIndex(void);
	void printInvertedIndex(void) const;

    public:
	InvertedIndex(const std::vector<std::string> &);
};
