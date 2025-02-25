#pragma once

#include <unordered_map>
#include <unordered_set>

#include "invertedindex.hpp"
#include "tilegrid.hpp"

class USetOperations {
    private:
	const std::string   &guess;
	const TileGrid	    &tileGrid;
	const InvertedIndex &invertedIndex;

	std::unordered_set<std::string>				   correct;
	std::unordered_map<char, std::unordered_set<std::string> > presentUMap;
	std::unordered_set<std::string>				   present;
	std::unordered_set<std::string>				   absent;

	std::unordered_set<std::string> result;

	void addCorrect(const std::unordered_set<std::string> &);
	void addPresent(const std::unordered_set<std::string> &, char);
	void addAbsent(char);
	void populate(void);
	void presentAggregate(void);

	void uset_union(std::unordered_set<std::string> &,
			const std::unordered_set<std::string> &);
	void uset_intersection(std::unordered_set<std::string> &,
			       const std::unordered_set<std::string> &);
	void uset_difference(std::unordered_set<std::string> &,
			     const std::unordered_set<std::string> &);

	std::string key(char, std::size_t) const;

	const std::unordered_set<std::string> &index(const std::string &) const;

    public:
	USetOperations(const std::string &, const TileGrid &,
		       const InvertedIndex &);
	std::unordered_set<std::string> execute(void);
};
