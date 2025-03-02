#pragma once

#include <string>
#include <unordered_set>

#include "invertedindex.hpp"
#include "tilegrid.hpp"
#include "usetoperations.hpp"

class WordlePlayer {
    private:
	static constexpr std::size_t MAX_DOCS = 15000;

	const std::unordered_set<std::string> words;

	std::string bestOpener;

	const InvertedIndex startIndex;

	USetOperations usetop;

	std::unordered_set<std::string> readWords(void) const;

	std::string setBestOpener(void) const;

	void printWords(void) const;

    public:
	WordlePlayer(void);

	InvertedIndex guess(const TileGrid &, const InvertedIndex &);
	InvertedIndex guess(const TileGrid &);

	const std::unordered_set<std::string> &getWords(void) const;

	void		   setUserOpener(std::string userOpener);
	const std::string &opener(void) const;
};
