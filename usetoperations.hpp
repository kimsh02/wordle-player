#pragma once

#include <unordered_map>
#include <unordered_set>

#include "invertedindex.hpp"
#include "tilegrid.hpp"

class USetOperations {
    private:
	const TileGrid	    *tileGrid;
	const InvertedIndex *invertedIndex;

	std::unordered_set<std::string>				   correct;
	std::unordered_map<char, std::unordered_set<std::string> > presentUMap;
	std::unordered_set<std::string>				   present;
	std::unordered_set<std::string>				   absent;
	std::unordered_set<std::string>				   result;

	void addCorrect(const std::unordered_set<std::string> &);
	void addPresent(const std::unordered_set<std::string> &, char);
	void addAbsent(char);
	void populate(void);
	void presentAggregate(void);
	void clear(void);

	void uset_union(std::unordered_set<std::string> &,
			const std::unordered_set<std::string> &);
	void uset_intersection(std::unordered_set<std::string> &,
			       const std::unordered_set<std::string> &);
	void uset_difference(std::unordered_set<std::string> &,
			     const std::unordered_set<std::string> &);

    public:
	void input(const TileGrid &, const InvertedIndex &);
	const std::unordered_set<std::string> &execute(void);
};
