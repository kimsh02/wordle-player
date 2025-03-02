#include "wordscorer.hpp"

#include <iostream>

#include "doclen.hpp"
#include "invertedindex.hpp"

WordScorer::WordScorer(const InvertedIndex &index)
	: index{ index }
{
}

const std::string &WordScorer::bestGuess(void)
{
	const std::string *bestGuess{};
	std::size_t	   bestScore{ 0 };

	std::unordered_set<char> letterSet;
	for (const auto &word : index.words()) {
		std::size_t currScore{ 0 };
		for (std::size_t i = 0; i < DOC_LEN; i++) {
			char letter = word[i];
			if (letterSet.contains(letter)) {
				// currScore += index.frequency(letter, i) / 2;
				continue;
			} else {
				currScore += index.frequency(letter, i);
			}
			letterSet.insert(letter);
		}
		letterSet.clear();
		if (currScore > bestScore) {
			bestScore = currScore;
			bestGuess = &word;
		}
	}
	return *bestGuess;
}

const std::string &WordScorer::firstWord(void)
{
	return *index.words().begin();
}
