#pragma once

#include <string>
#include <vector>

class WordlePlayer {
    private:
	static constexpr std::size_t MAX_DOCS = 15000;

	std::vector<std::string> words;

	void readWords(void);
	void printWords(void) const;

    public:
	WordlePlayer(void);
};
