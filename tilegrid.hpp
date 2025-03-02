#pragma once

#include <array>
#include <string>
#include <unordered_map>

#include "doclen.hpp"

class TileGrid {
    public:
	enum TileState { e, g, y, x };

	void feedback(const std::string &);

	const std::array<TileState, DOC_LEN> &getFeedback(void) const;
	const std::string		     &getGuess(void) const;
	const std::string		     &getWordOfDay(void) const;

	void setWordOfDay(const std::string &);
	bool won(void) const;
	void printTileGrid(void) const;

    private:
	static constexpr std::size_t RGB_CHAN = 6;
	static constexpr std::array<int, RGB_CHAN> G{
		83, 141, 78, 248, 248, 248
	};
	static constexpr std::array<int, RGB_CHAN> Y{ 181, 159, 59,
						      248, 248, 248 };
	static constexpr std::array<int, RGB_CHAN> X{
		58, 58, 60, 255, 255, 255
	};

	const std::string *wordOfDay;
	const std::string *guess;

	std::unordered_map<char, std::size_t> letterCounter;

	std::array<TileState, DOC_LEN> tiles;

	void makeLetterCounter(void);

	void printTile(char, const std::array<int, RGB_CHAN> &) const;

	void makeTiles(void);
	void clearTiles(void);
};
