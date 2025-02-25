#include "usetoperations.hpp"

#include <ranges>
#include <unordered_set>

#include "doclen.hpp"

USetOperations::USetOperations(const std::string   &guess,
			       const TileGrid	   &tileGrid,
			       const InvertedIndex &invertedIndex)
	: guess{ guess }
	, tileGrid{ tileGrid }
	, invertedIndex{ invertedIndex }
{
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
			uset_union(presentUMap[letter], index(key(letter, i)));
		}
	}
	uset_difference(presentUMap[letter], b);
}

void USetOperations::addAbsent(char letter)
{
	if (!presentUMap.contains(letter)) {
		for (std::size_t i = 0; i < DOC_LEN; i++) {
			uset_union(absent, index(key(letter, i)));
		}
	}
}

const std::unordered_set<std::string> &
USetOperations::index(const std::string &key) const
{
	return invertedIndex.get().first.at(key);
}

std::string USetOperations::key(char letter, std::size_t pos) const
{
	return std::string{ letter, static_cast<char>(pos + '0') };
}

void USetOperations::populate(void)
{
	for (std::size_t pos = 0; pos < DOC_LEN; pos++) {
		auto	    tileState = tileGrid.get()[pos];
		char	    letter    = guess[pos];
		const auto &b{ index(key(letter, pos)) };
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

std::unordered_set<std::string> USetOperations::execute(void)
{
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
		auto result{ validWords };
		uset_difference(result, absent);
		return result;
	}
}
