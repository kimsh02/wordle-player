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
	static constexpr std::size_t MAX_KEYS = 130;

	/* By value, and std::move */
	const std::unordered_set<std::string> docs;
	const IndexPair			      indexPair;

	std::unordered_set<std::string> makeKeys(void) const;
	IndexPair			makeIndexPair(void) const;

	// void printIndices(void) const;

    public:
	InvertedIndex(const std::unordered_set<std::string> &);
	// InvertedIndex(std::unordered_set<std::string>);

	void printInvertedIndex(void) const;

	const IndexPair			      &get(void) const;
	const std::unordered_set<std::string> &words(void) const;
	// std::unordered_set<std::string>	      &giveWords(void);

	std::string key(char, std::size_t) const;

	const std::unordered_set<std::string> &at(char, std::size_t) const;
	bool contains(char, std::size_t) const;

	std::size_t frequency(char, std::size_t) const;
};
