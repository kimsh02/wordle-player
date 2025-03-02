#include "wordle.hpp"

#include <curl/curl.h>

#include <algorithm>
#include <cstdlib>
#include <iostream>

#include "doclen.hpp"
#include "invertedindex.hpp"
#include "nytimesfetcher.hpp"
#include "tilegrid.hpp"
#include "utility.hpp"
#include "wordleplayer.hpp"
#include "wordscorer.hpp"

WordlePlayer Wordle::wordlePlayer{};

Wordle::Wordle(int argc, const char *const *argv)
	: prog{ *argv }
	, wordOfDay{ NYTimesFetcher{}.fetch() }
{
	parseArgs(argc, argv);
}

void Wordle::play(void)
{
	tileGrid.feedback(wordlePlayer.opener());
	play_helper(wordlePlayer.guess(tileGrid), MAX_TRIES - 1);
}

void Wordle::play_helper(InvertedIndex index, std::size_t tries)
{
	// printSet(index.words());
	std::cout << "\n";
	if (tries == 0) {
		std::cerr
			<< "ERROR: Wordle player failed to guess the word in at most six tries.\n";
	}
	tileGrid.feedback(WordScorer{ index }.firstWord());
	if (tileGrid.won()) {
		std::cout << "\n";
		return;
	} else {
		play_helper(wordlePlayer.guess(tileGrid, index), tries - 1);
	}
}

void Wordle::peek(void)
{
	std::cout << "Word of the day is ";
	tileGrid.feedback(wordOfDay);
	std::cout
		<< " ( from https://www.nytimes.com/games/wordle/index.html )!\n";
}

void Wordle::benchmark(bool) const
{
}

void Wordle::help(void) const
{
	std::cout
		<< "Usage: " << prog << " COMMAND\n\n"
		<< "Fast and efficient player for Wordle (nytimes.com).\n\n"
		<< "Commands:\n"
		<< "  play [WORD]    Solve word of the day from Wordle (nytimes.com) or solve user-picked 5-letter word\n"
		<< "  peek           Show word of the day\n"
		<< "  bm             Benchmark performance\n"
		<< "  bmv            Benchmark performance verbose\n"
		<< "  help           Show this help message and exit\n"
		<< "\nExamples:\n"
		<< "  " << prog << " play\n"
		<< "  " << prog << " play SHARE\n";
}

void Wordle::setUserWordOfDay(std::string userWord)
{
	/* Check if it's valid 5 letter word */

	if (userWord.size() == DOC_LEN) {
		std::transform(userWord.begin(),
			       userWord.end(),
			       userWord.begin(),
			       ::tolower);
		if (wordlePlayer.getWords().contains(userWord)) {
			wordOfDay = userWord;
			return;
		}
	}
	help();
	std::exit(EXIT_SUCCESS);
}

void Wordle::parseArgs(int argc, const char *const *argv)
{
	if (argc != 2 && argc != 3) {
		help();
	} else {
		std::string arg{ argv[1] };
		if (arg == "play") {
			if (argc == 3) {
				setUserWordOfDay(std::string{ argv[2] });
			}
			tileGrid.setWordOfDay(wordOfDay);
			play();
		} else if (arg == "peek") {
			peek();
		} else if (arg == "bm") {
			benchmark(false);
		} else if (arg == "bmv") {
			benchmark(true);
		} else {
			help();
		}
	}
}

void Wordle::setNYTimesWordOfDay(void)
{
	wordOfDay = NYTimesFetcher{}.fetch();

	if (wordOfDay.size() == 0) {
		std::cerr
			<< "ERROR: cURL failed to start or failed to retrieve nytimes.com word of the day.\n";
		std::exit(EXIT_FAILURE);
	}
}
