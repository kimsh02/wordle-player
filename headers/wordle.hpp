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

	std::size_t play(bool);
	std::size_t play_helper(InvertedIndex, std::size_t, bool);
	void	    peek(void);
	void	    benchmark(bool);
	void	    help(void) const;

	void parseArgs(int argc, const char *const *argv);
	void setUserWordOfDay(const std::string &);
	void validateWord(std::string &userWord) const;
	void setNYTimesWordOfDay(void);

    public:
	Wordle(int, const char *const *);
};
