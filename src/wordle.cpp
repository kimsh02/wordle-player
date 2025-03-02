#include "wordle.hpp"

#include <curl/curl.h>

#include <algorithm>
#include <chrono>
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

std::size_t Wordle::play(bool verbose)
{
	tileGrid.feedback(wordlePlayer.opener());
	return play_helper(wordlePlayer.guess(tileGrid), 2, verbose);
}

std::size_t Wordle::play_helper(InvertedIndex index, std::size_t tries,
				bool verbose)
{
	// printSet(index.words());
	if (verbose) {
		tileGrid.printTileGrid();
		std::cout << "\n";
	}

	tileGrid.feedback(WordScorer{ index }.bestGuess(tries));
	// tileGrid.feedback(WordScorer{ index }.firstWord());
	if (tileGrid.won()) {
		tileGrid.printTileGrid();
		return tries;
	}
	return play_helper(
		wordlePlayer.guess(tileGrid, index), tries + 1, verbose);
}

void Wordle::peek(void)
{
	std::cout << "Word of the day is ";
	tileGrid.feedback(wordOfDay);
	tileGrid.printTileGrid();
	std::cout
		<< " ( from https://www.nytimes.com/games/wordle/index.html )!\n";
}

void Wordle::benchmark(bool verbose)
{
	auto	    start{ std::chrono::high_resolution_clock::now() };
	std::size_t totalTries{ 0 };
	for (const auto &word : wordlePlayer.getWords()) {
		tileGrid.setWordOfDay(word);
		std::cout
			<< "-----------------------------------------------------\n";
		std::size_t tries = play(verbose);
		std::cout << " guessed in " << tries << " tries.\n";
		totalTries += tries;
	}
	auto end{ std::chrono::high_resolution_clock::now() };
	std::chrono::duration<double> elapsed = end - start;
	std::size_t numWords{ wordlePlayer.getWords().size() };
	std::cout << "-----------------------------------------------------\n";
	std::cout << "Opener:                 " << wordlePlayer.opener()
		  << "\n";
	std::cout << "Words solved:           " << numWords << "\n";
	std::cout << "Total time:             " << elapsed.count()
		  << " seconds\n";
	std::cout << "Average time per word:  " << elapsed.count() / numWords
		  << " seconds\n";
	std::cout << "Average tries per word: "
		  << static_cast<double>(totalTries) / numWords << "\n";
}

void Wordle::help(void) const
{
	std::cout
		<< "Usage: " << prog << " COMMAND\n\n"
		<< "Fast and efficient player for Wordle (nytimes.com).\n\n"
		<< "Commands:\n"
		<< "  play [WORD]    Solve word of the day from Wordle (nytimes.com) or solve your own (valid) 5-letter word\n"
		<< "  peek           Show word of the day\n"
		<< "  bm [OPENER]    Benchmark performance with default opener (PARES) or with your own (valid) opener\n"
		<< "  bmv [OPENER]   Benchmark performance verbose\n"
		<< "  help           Show this help message and exit\n"
		<< "\nExamples:\n"
		<< "  " << prog << " play\n"
		<< "  " << prog << " play SHARE\n"
		<< "  " << prog << " bm\n"
		<< "  " << prog << " bm PROXY\n";
}

void Wordle::setUserWordOfDay(const std::string &wordOfDay)
{
	this->wordOfDay = std::move(wordOfDay);
}

void Wordle::validateWord(std::string &userWord) const
{
	/* Check if it's valid 5 letter word */

	if (userWord.size() == DOC_LEN) {
		std::transform(userWord.begin(),
			       userWord.end(),
			       userWord.begin(),
			       ::tolower);
		if (wordlePlayer.getWords().contains(userWord)) {
			// wordOfDay = userWord;
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
				std::string userWord{ argv[2] };
				validateWord(userWord);
				setUserWordOfDay(userWord);
			}
			tileGrid.setWordOfDay(wordOfDay);
			std::cout
				<< "-----------------------------------------------------\n";
			play(true);
			std::cout
				<< "\n-----------------------------------------------------\n";
		} else if (arg == "peek" && argc == 2) {
			tileGrid.setWordOfDay(wordOfDay);
			peek();
		} else if (arg == "bm") {
			if (argc == 3) {
				std::string userOpener{ argv[2] };
				validateWord(userOpener);
				wordlePlayer.setUserOpener(userOpener);
			}
			benchmark(false);
		} else if (arg == "bmv") {
			if (argc == 3) {
				std::string userOpener{ argv[2] };
				validateWord(userOpener);
				wordlePlayer.setUserOpener(userOpener);
			}
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
