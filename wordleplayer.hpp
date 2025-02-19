#pragma once

#include <string>
#include <vector>

class WordlePlayer {
    private:
	static const std::size_t MAX_WORDS = 15000;

	const int	   argc;
	const char *const *argv;

	std::vector<std::string> words;

	void readWords(void);
	void printWords(void);

    public:
	WordlePlayer(int, const char *const *);
};
