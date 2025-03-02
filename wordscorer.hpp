#pragma once

#include "invertedindex.hpp"

class WordScorer {
    private:
	const InvertedIndex &index;

    public:
	WordScorer(const InvertedIndex &);
	const std::string &bestGuess(void);
	const std::string &firstWord(void);
};
