#include "wordleplayer.hpp"

#include <cstdlib>
#include <fstream>
#include <iostream>

#include "invertedindex.hpp"

void WordlePlayer::readWords(void)
{
	std::ifstream f("../assets/valid-wordle-words.txt");
	if (!f) {
		std::cerr << "ERROR: Unable to open file!\n";
		std::exit(EXIT_FAILURE);
	}
	words.reserve(MAX_WORDS);
	std::string word;
	while (std::getline(f, word)) {
		words.push_back(std::move(word));
	}
	f.close();
}

void WordlePlayer::printWords(void)
{
	for (const auto &word : words) {
		std::cout << word << "\n";
	}
}

WordlePlayer::WordlePlayer(int argc, const char *const *argv)
	: argc{ argc }
	, argv{ argv }
{
	readWords();
	printWords();

	// TESTING
	InvertedIndex i{ words };
}
