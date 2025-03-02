#include "wordscorer.hpp"

#include <iostream>

#include "doclen.hpp"
#include "invertedindex.hpp"

WordScorer::WordScorer(const InvertedIndex &index)
	: index{ index }
{
}

bool WordScorer::isVowel(char letter) const
{
	std::array<char, 6> vowels{ 'a', 'e', 'i', 'o', 'u', 'y' };
	for (std::size_t i = 0; i < vowels.size(); i++) {
		if (letter == vowels[i]) {
			return true;
		}
	}
	return false;
}

const std::string &WordScorer::bestGuess(std::size_t tries)
{
	if (tries == 2 || tries == 4) {
		const std::string *bestGuess{};
		std::size_t	   bestScore{ 0 };

		std::unordered_set<char> letterSet;
		for (const auto &word : index.words()) {
			std::size_t currScore{ 0 };
			for (std::size_t i = 0; i < DOC_LEN; i++) {
				char letter = word[i];
				if (letterSet.contains(letter)) {
					break;
				} else {
					if (isVowel(letter)) {
						currScore +=
							index.frequency(letter,
									i) *
							5;
					} else {
						currScore += index.frequency(
							letter, i);
					}
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
	} else {
		return firstWord();
	}
}

const std::string &WordScorer::firstWord(void)
{
	return *index.words().begin();
}
