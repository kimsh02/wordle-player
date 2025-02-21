#include "wordleplayer.hpp"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "invertedindex.hpp"

void WordlePlayer::help(void) const
{
	std::cout
		<< "Usage: " << argv[0] << " COMMAND\n\n"
		<< "Fast and efficient player for Wordle (nytimes.com).\n\n"
		<< "Commands:\n"
		<< "  play [WORD]    Solve word of the day from Wordle (nytimes.com) or solve user-picked word\n"
		<< "  peek           Show word of the day\n"
		<< "  bm             Benchmark performance\n"
		<< "  bmv            Benchmark performance verbose\n"
		<< "  help           Show this help message and exit\n"
		<< "\nExamples:\n"
		<< "  " << argv[0] << " play\n"
		<< "  " << argv[0] << " play SHARE\n";
}

void WordlePlayer::play(const std::string & = wordOfDay) const
{
}

void WordlePlayer::peek(void) const
{
}

void WordlePlayer::benchmark(bool) const
{
}

void WordlePlayer::validateUserWord(std::string userWord) const
{
	if (userWord.size() == 5) {
		std::transform(userWord.begin(),
			       userWord.end(),
			       userWord.begin(),
			       ::tolower);
		play(userWord);

	} else {
		help();
	}
}

void WordlePlayer::parseArgs(void)
{
	if (argc != 2 && argc != 3) {
		help();
	} else {
		std::string arg{ argv[1] };
		if (arg == "play") {
			if (argc == 2) {
				play();
			} else {
				validateUserWord(std::string{ argv[2] });
			}
		} else if (arg == "peek") {
			peek();
		} else if (arg == "bm") {
			benchmark(false);
		} else if (arg == "bmv") {
			benchmark(true);
		} else if (arg == "help") {
			help();
		} else {
		}
	}
	std::exit(EXIT_SUCCESS);
}

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

void WordlePlayer::printWords(void) const
{
	for (const auto &word : words) {
		std::cout << word << "\n";
	}
}

WordlePlayer::WordlePlayer(int argc, const char *const *argv)
	: argc{ argc }
	, argv{ argv }
{
	// parseArgs();

	readWords();
	printWords();

	InvertedIndex i{ words };
}
