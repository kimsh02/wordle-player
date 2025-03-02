#pragma once

#include "invertedindex.hpp"

class WordScorer {
    private:
	const InvertedIndex &index;

	bool isVowel(char letter) const;

    public:
	WordScorer(const InvertedIndex &);
	const std::string &bestGuess(std::size_t);
	const std::string &firstWord(void);
};
