#include "wordleplayer.hpp"

#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "doclen.hpp"
#include "invertedindex.hpp"

void WordlePlayer::readWords(void)
{
	std::ifstream f("../assets/valid-wordle-words.txt");
	if (!f) {
		std::cerr << "ERROR: Unable to open file!\n";
		std::exit(EXIT_FAILURE);
	}
	words.reserve(MAX_DOCS);
	std::string word{};
	word.reserve(DOC_LEN);
	while (std::getline(f, word)) {
		words.push_back(std::move(word));
	}
	f.close();
}

void WordlePlayer::printWords(void) const
{
	for (const auto &word : words) {
		std::cout << word << "\n";
	}
}

WordlePlayer::WordlePlayer(void)
{
	readWords();
	printWords();

	InvertedIndex invertedIndex{ words };
}
