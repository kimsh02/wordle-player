#pragma once

#include <unordered_map>
#include <unordered_set>

#include "invertedindex.hpp"
#include "tilegrid.hpp"

class USetOperations {
    private:
	/* std::array */
	using KeyMap =
		std::unordered_map<char, std::unordered_set<std::size_t> >;
	using USet = std::unordered_set<std::string>;

	const TileGrid	    *tileGrid;
	const InvertedIndex *invertedIndex;

	KeyMap correctKeys;
	KeyMap presentKeys;
	KeyMap absentKeys;

	USet correctUSet;
	USet presentUSet;
	USet absentUSet;

	void clear(void);
	void makeKeys(void);
	void addKey(char, std::size_t, KeyMap &);
	void makeCorrectUSet(void);
	void makePresentUSet(void);
	void makeAbsentUSet(void);

	void uset_union(std::unordered_set<std::string> &,
			const std::unordered_set<std::string> &);
	void uset_intersection(std::unordered_set<std::string> &,
			       const std::unordered_set<std::string> &);
	void uset_difference(std::unordered_set<std::string> &,
			     const std::unordered_set<std::string> &);

    public:
	void input(const TileGrid &, const InvertedIndex &);
	std::unordered_set<std::string> execute(void);
};
