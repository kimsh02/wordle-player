#include "tilegrid.hpp"

#include <algorithm>
#include <array>
#include <iostream>
#include <unordered_map>

#include "doclen.hpp"

TileGrid::TileGrid(const std::string &wordOfDay)
	: wordOfDay{ wordOfDay }
	, letterCounter{ makeLetterCounter() }
{
}

std::unordered_map<char, std::size_t> TileGrid::makeLetterCounter(void) const
{
	std::unordered_map<char, std::size_t> letterCounter{};
	for (const auto &letter : wordOfDay) {
		letterCounter[letter] += 1;
	}
	return letterCounter;
}

void TileGrid::makeTiles(const std::string &guess)
{
	clearTiles();

	auto letterCounter{ this->letterCounter };
	for (std::size_t i = 0; i < DOC_LEN; i++) {
		if (guess[i] == wordOfDay[i]) {
			letterCounter[wordOfDay[i]] -= 1;
			this->tiles[i] = g;
		}
	}

	for (std::size_t i = 0; i < DOC_LEN; i++) {
		if (this->tiles[i] == e) {
			if (letterCounter[guess[i]] > 0) {
				letterCounter[wordOfDay[i]] -= 1;
				this->tiles[i] = y;
			} else {
				this->tiles[i] = x;
			}
		}
	}
}

void TileGrid::feedback(const std::string &guess)
{
	makeTiles(guess);
	printTileGrid(guess, tiles);
}

const std::array<TileGrid::TileState, DOC_LEN> &TileGrid::get(void) const
{
	return tiles;
}

void TileGrid::clearTiles(void)
{
	std::ranges::fill(tiles, e);
}

void TileGrid::printTileGrid(const std::string			  &guess,
			     const std::array<TileState, DOC_LEN> &tiles) const
{
	for (std::size_t i = 0; i < DOC_LEN; i++) {
		switch (tiles[i]) {
		case g:
			printTile(guess[i], G);
			break;
		case y:
			printTile(guess[i], Y);
			break;
		case x:
			printTile(guess[i], X);
			break;
		default:
			/* Should not go to default*/
		}
	}
	std::cout << "\n";
}

void TileGrid::printTile(char letter, const std::array<int, RGB_CHAN> &c) const
{
	std::cout << "\033[48;2;" << c[0] << ";" << c[1] << ";" << c[2] << "m"
		  << "\033[38;2;" << c[3] << ";" << c[4] << ";" << c[5] << "m"
		  << " " << letter << " " << "\033[0m";
}
