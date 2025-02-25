#include "wordle.hpp"

#include <curl/curl.h>

#include <cstdlib>
#include <iostream>

#include "doclen.hpp"
#include "nytimesfetcher.hpp"
#include "wordleplayer.hpp"

Wordle::Wordle(int argc, const char *const *argv)
	: prog{ *argv }
{
	parseArgs(argc, argv);
}

void Wordle::play(void) const
{
	WordlePlayer wp{};
}

void Wordle::peek(void) const
{
}

void Wordle::benchmark(bool) const
{
}

void Wordle::openers(void) const
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
		<< "  openers        Show 10 best openers ranked by positional frequency of letters\n"
		<< "  bm             Benchmark performance\n"
		<< "  bmv            Benchmark performance verbose\n"
		<< "  help           Show this help message and exit\n"
		<< "\nExamples:\n"
		<< "  " << prog << " play\n"
		<< "  " << prog << " play SHARE\n";
}

void Wordle::setUserWordOfDay(std::string userWord)
{
	if (userWord.size() == DOC_LEN) {
		std::transform(userWord.begin(),
			       userWord.end(),
			       userWord.begin(),
			       ::tolower);
		wordOfDay = std::move(userWord);

	} else {
		help();
		std::exit(EXIT_SUCCESS);
	}
}

void Wordle::parseArgs(int argc, const char *const *argv)
{
	if (argc != 2 && argc != 3) {
		help();
	} else {
		std::string arg{ argv[1] };
		if (arg == "play") {
			if (argc == 2) {
				setNYTimesWordOfDay();
			} else {
				setUserWordOfDay(std::string{ argv[2] });
			}
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
