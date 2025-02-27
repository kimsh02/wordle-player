#include "usetoperations.hpp"

#include <ranges>
#include <unordered_set>

#include "doclen.hpp"

void USetOperations::input(const TileGrid      &tileGrid,
			   const InvertedIndex &invertedIndex)
{
	this->tileGrid	    = &tileGrid;
	this->invertedIndex = &invertedIndex;
}

void USetOperations::uset_union(std::unordered_set<std::string>	      &a,
				const std::unordered_set<std::string> &b)
{
	a.insert(b.begin(), b.end());
}

void USetOperations::uset_intersection(std::unordered_set<std::string>	     &a,
				       const std::unordered_set<std::string> &b)
{
	for (auto it = a.begin(); it != a.end();) {
		if (!b.contains(*it)) {
			it = a.erase(it);
		} else {
			it++;
		}
	}
}

void USetOperations::uset_difference(std::unordered_set<std::string>	   &a,
				     const std::unordered_set<std::string> &b)
{
	for (auto it = a.begin(); it != a.end();) {
		if (b.contains(*it)) {
			it = a.erase(it);
		} else {
			it++;
		}
	}
}

void USetOperations::addCorrect(const std::unordered_set<std::string> &b)
{
	if (correct.empty()) {
		uset_union(correct, b);
	} else {
		uset_intersection(correct, b);
	}
}

void USetOperations::addPresent(const std::unordered_set<std::string> &b,
				char				       letter)
{
	if (presentUMap[letter].empty()) {
		for (std::size_t i = 0; i < DOC_LEN; i++) {
			uset_union(presentUMap[letter],
				   invertedIndex->at(letter, i));
		}
	}
	uset_difference(presentUMap[letter], b);
}

void USetOperations::addAbsent(char letter)
{
	if (!presentUMap.contains(letter)) {
		for (std::size_t i = 0; i < DOC_LEN; i++) {
			uset_union(absent, invertedIndex->at(letter, i));
		}
	}
}

void USetOperations::populate(void)
{
	for (std::size_t pos = 0; pos < DOC_LEN; pos++) {
		const auto &tileState = tileGrid->get()[pos];
		char	    letter    = tileGrid->word()[pos];
		const auto &b{ invertedIndex->at(letter, pos) };
		switch (tileState) {
		case TileGrid::g:
			addCorrect(b);
			break;
		case TileGrid::y:
			addPresent(b, letter);
			break;
		case TileGrid::x:
			addAbsent(letter);
			break;
		default:
			break;
		}
	}
}

void USetOperations::presentAggregate(void)
{
	if (!presentUMap.empty()) {
		for (const auto &p : std::views::values(presentUMap)) {
			uset_union(present, p);
		}
	}
}

void USetOperations::clear(void)
{
	correct.clear();
	presentUMap.clear();
	present.clear();
	absent.clear();
	result.clear();
}

const std::unordered_set<std::string> &USetOperations::execute(void)
{
	clear();
	populate();
	presentAggregate();
	if (!correct.empty()) {
		if (!present.empty()) {
			uset_intersection(correct, present);
		}
		if (!absent.empty()) {
			uset_difference(correct, absent);
		}
		return correct;
	} else if (!present.empty()) {
		if (!absent.empty()) {
			uset_difference(present, absent);
		}
		return present;
	} else {
		// auto result{ invertedIndex->words() };
		result = invertedIndex->words();
		uset_difference(result, absent);
		return result;
	}
}
