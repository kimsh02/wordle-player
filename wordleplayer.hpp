#pragma once

#include <string>
#include <vector>

class WordlePlayer {
    private:
	static constexpr std::size_t MAX_WORDS = 15000;

	const int	   argc;
	const char *const *argv;

	static std::string wordOfDay;

	std::vector<std::string> words;

	void play(const std::string &) const;
	void peek(void) const;
	void benchmark(bool) const;
	void help(void) const;
	void validateUserWord(std::string) const;

	void readWords(void);
	void printWords(void) const;

	void parseArgs(void);

    public:
	WordlePlayer(int, const char *const *);
};
