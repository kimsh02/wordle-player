#pragma once

#include <string>

#include "invertedindex.hpp"
#include "tilegrid.hpp"
#include "wordleplayer.hpp"

class Wordle {
    private:
	static constexpr std::size_t MAX_TRIES = 6;
	static WordlePlayer	     wordlePlayer;

	const char *const prog;

	std::string wordOfDay;

	TileGrid tileGrid;

	void play(void);
	void play_helper(InvertedIndex, std::size_t);
	void peek(void);
	void benchmark(bool) const;
	void help(void) const;

	void parseArgs(int argc, const char *const *argv);
	void setUserWordOfDay(std::string);
	void setNYTimesWordOfDay(void);

    public:
	Wordle(int, const char *const *);
};
