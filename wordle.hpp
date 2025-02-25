#pragma once

#include <string>

class Wordle {
    private:
	std::string wordOfDay;

	const char *const prog;

	void play(void) const;
	void peek(void) const;
	void openers(void) const;
	void benchmark(bool) const;
	void help(void) const;

	void parseArgs(int argc, const char *const *argv);
	void setUserWordOfDay(std::string);
	void setNYTimesWordOfDay(void);

    public:
	Wordle(int, const char *const *);
};
