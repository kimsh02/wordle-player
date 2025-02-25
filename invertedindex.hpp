#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

using IndexPair = std::pair<
	std::unordered_map<std::string, std::unordered_set<std::string> >,
	std::unordered_map<std::string, std::size_t> >;

class InvertedIndex {
    private:
	static constexpr std::size_t MAX_INDICES = 130;

	const std::unordered_set<std::string> &docs;
	const IndexPair			       indexPair;

	std::unordered_set<std::string> makeIndices(void);
	IndexPair			makeIndexPair(void);

	// void printIndices(void) const;
	void printInvertedIndex(void) const;

    public:
	InvertedIndex(const std::unordered_set<std::string> &);

	const IndexPair			      &get(void) const;
	const std::unordered_set<std::string> &words(void) const;
};
