#pragma once

#include <string>
#include <unordered_set>

#include "invertedindex.hpp"

class WordlePlayer {
    private:
	static constexpr std::size_t MAX_DOCS = 15000;

	const std::unordered_set<std::string> words;

	const std::string opener;

	const InvertedIndex startIndex;

	std::unordered_set<std::string> readWords(void);

	std::string setBestOpener(void);

	void printWords(void) const;

    public:
	WordlePlayer(void);
	std::string guess(void);
};
