#include "wordleplayer.hpp"

#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <unordered_set>

#include "doclen.hpp"
#include "invertedindex.hpp"

std::unordered_set<std::string> WordlePlayer::readWords(void)
{
	std::unordered_set<std::string> words{};

	std::ifstream f("../assets/valid-wordle-words.txt");
	if (!f) {
		std::cerr << "ERROR: Unable to open file!\n";
		std::exit(EXIT_FAILURE);
	}
	words.reserve(MAX_DOCS);
	std::string word{};
	word.reserve(DOC_LEN);
	while (std::getline(f, word)) {
		words.insert(std::move(word));
	}
	f.close();
	return words;
}

void WordlePlayer::printWords(void) const
{
	for (const auto &word : words) {
		std::cout << word << "\n";
	}
}

// TODO
std::string WordlePlayer::setBestOpener(void)
{
	return std::string{ "" };
}

WordlePlayer::WordlePlayer(void)
	: words{ readWords() }
	, opener{ setBestOpener() }
	, startIndex{ words }

{
	printWords();
}
