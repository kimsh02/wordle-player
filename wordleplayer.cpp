#include "wordleplayer.hpp"

#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <unordered_set>

#include "doclen.hpp"
#include "invertedindex.hpp"
#include "tilegrid.hpp"
#include "usetoperations.hpp"
#include "wordscorer.hpp"

std::unordered_set<std::string> WordlePlayer::readWords(void) const
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

std::string WordlePlayer::setBestOpener(void) const
{
	/* Hardcoded to PARES from word scoring entire list */
	return std::string{ "pares" };
}

WordlePlayer::WordlePlayer(void)
	: words{ readWords() }
	, bestOpener{ setBestOpener() }
	, startIndex{ words }

{
	// printWords();
}

InvertedIndex WordlePlayer::guess(const TileGrid      &tileGrid,
				  const InvertedIndex &index)
{
	usetop.input(tileGrid, index);
	return InvertedIndex{ usetop.execute() };
}

InvertedIndex WordlePlayer::guess(const TileGrid &tileGrid)
{
	usetop.input(tileGrid, startIndex);
	return InvertedIndex{ usetop.execute() };
}

const std::string &WordlePlayer::opener(void) const
{
	return bestOpener;
}
