#include "usetoperations.hpp"

#include <iostream>
#include <ranges>
#include <unordered_set>
#include <vector>

#include "doclen.hpp"
#include "utility.hpp"

using USO = USetOperations;

void USO::input(const TileGrid &tileGrid, const InvertedIndex &invertedIndex)
{
	this->tileGrid	    = &tileGrid;
	this->invertedIndex = &invertedIndex;
}

void USO::uset_union(std::unordered_set<std::string>	   &a,
		     const std::unordered_set<std::string> &b)
{
	a.insert(b.begin(), b.end());
}

void USO::uset_intersection(std::unordered_set<std::string>	  &a,
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

void USO::uset_difference(std::unordered_set<std::string>	&a,
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

// void USO::addCorrect(const std::unordered_set<std::string> &b)
// {
// 	if (correct.empty()) {
// 		uset_union(correct, b);
// 	} else {
// 		uset_intersection(correct, b);
// 	}
// }

// void USO::addPresent(char letter, std::size_t pos)
// {
// 	presentUMap[letter].insert(pos);
// }

// void USO::addAbsent(char letter, std::size_t pos)
// {
// 	if (!presentUMap.contains(letter)) {
// 		for (std::size_t i = 0; i < DOC_LEN; i++) {
// 			if (invertedIndex->contains(letter, i)) {
// 				uset_union(absent,
// 					   invertedIndex->at(letter, i));
// 			}
// 		}
// 	} else {
// 		uset_union(absent, invertedIndex->at(letter, pos));
// 	}
// }

void USO::makeCorrectUSet(void)
{
	for (const auto &[k, v] : correctKeys) {
		for (const auto &pos : v) {
			const auto &b{ invertedIndex->at(k, pos) };
			if (correctUSet.empty()) {
				uset_union(correctUSet, b);
			} else {
				uset_intersection(correctUSet, b);
			}
		}
	}
}

void USO::makePresentUSet(void)
{
	for (const auto &[letter, disallowed] : presentKeys) {
		USet tmp{};

		if (disallowed.size() == 1) {
			// for (const auto &pos : disallowed) {
			for (std::size_t i = 0; i < DOC_LEN; i++) {
				if (!disallowed.contains(i) &&
				    invertedIndex->contains(letter, i)) {
					if (correctKeys.contains(letter)) {
						if (!correctKeys[letter]
							     .contains(i)) {
							uset_union(
								tmp,
								invertedIndex->at(
									letter,
									i));
						}
					} else {
						uset_union(tmp,
							   invertedIndex->at(
								   letter, i));
					}
				}
			}
			// }
		} else if (disallowed.size() == 2) {
			std::vector<USet> usets;
			std::size_t	  idx = 0;
			// for (const auto &pos : disallowed) {
			for (std::size_t i = 0; i < DOC_LEN; i++) {
				if (!disallowed.contains(i) &&
				    invertedIndex->contains(letter, i)) {
					if (correctKeys.contains(letter)) {
						if (!correctKeys[letter]
							     .contains(i)) {
							usets.push_back(
								invertedIndex->at(
									letter,
									i));
							idx++;
						}
					} else {
						usets.push_back(
							invertedIndex->at(
								letter, i));
						idx++;
					}
				}
			}
			// }
			if (idx == 2) {
				uset_intersection(usets[0], usets[1]);
				tmp = usets[0];
			} else {
				USet one{ usets[0] }, two{ usets[0] },
					three{ usets[1] };
				uset_intersection(one, usets[1]);
				uset_intersection(two, usets[2]);
				uset_intersection(three, usets[2]);
				uset_union(one, two);
				uset_union(one, three);
				tmp = one;
			}
		}

		if (presentUSet.empty()) {
			uset_union(presentUSet, tmp);
		} else {
			uset_intersection(presentUSet, tmp);
		}
		// if (correctKeys.contains(letter)) {
		// 	std::cout << "\nhere\n";
		// 	for (const auto &pos : correctKeys[letter]) {
		// 		uset_intersection(presentUSet,
		// 				  invertedIndex->at(letter,
		// 						    pos));
		// 	}
		// 	printSet(presentUSet);
		// }
	}
}

void USO::makeAbsentUSet(void)
{
	for (const auto &[letter, v] : absentKeys) {
		if (correctKeys.contains(letter) ||
		    presentKeys.contains(letter)) {
			for (const auto &pos : v) {
				uset_union(absentUSet,
					   invertedIndex->at(letter, pos));
			}
		} else {
			for (std::size_t i = 0; i < DOC_LEN; i++) {
				if (invertedIndex->contains(letter, i)) {
					uset_union(absentUSet,
						   invertedIndex->at(letter,
								     i));
				}
			}
		}
	}

	for (const auto &[letter, v] : presentKeys) {
		for (const auto &pos : v) {
			uset_union(absentUSet, invertedIndex->at(letter, pos));
		}
	}
}

void USO::addKey(char letter, std::size_t pos, KeyMap &km)
{
	km[letter].insert(pos);
}

void USO::makeKeys(void)
{
	for (std::size_t pos = 0; pos < DOC_LEN; pos++) {
		const auto &tileState = tileGrid->getFeedback()[pos];
		char	    letter    = tileGrid->getGuess()[pos];
		// const auto &b{ invertedIndex->at(letter, pos) };
		switch (tileState) {
		case TileGrid::g:
			addKey(letter, pos, correctKeys);
			break;
		case TileGrid::y:
			addKey(letter, pos, presentKeys);
			break;
		case TileGrid::x:
			addKey(letter, pos, absentKeys);
			break;
		default:
			break;
		}
	}
}

// void USO::presentFilter(void)
// {
// 	for (const auto &[key, value] : presentUMap) {
// 		for (std::size_t i = 0; i < DOC_LEN; i++) {
// 			if (value.contains(i)) {
// 				uset_union(absent, invertedIndex->at(key, i));
// 			} else if (invertedIndex->contains(key, i)) {
// 				uset_union(present, invertedIndex->at(key, i));
// 			}
// 		}
// 	}
// }

void USO::clear(void)
{
	correctKeys.clear();
	presentKeys.clear();
	absentKeys.clear();

	correctUSet.clear();
	presentUSet.clear();
	absentUSet.clear();
}

std::unordered_set<std::string> USO::execute(void)
{
	clear();
	makeKeys();
	makeCorrectUSet();
	makePresentUSet();
	makeAbsentUSet();

	// std::cout << "\nCORRECT: ";
	// printSet(correctUSet);
	// std::cout << "\n\n";
	// std::cout << "PRESENT: ";
	// printSet(presentUSet);
	// std::cout << "\n\n";
	// std::cout << "ABSENT: ";
	// printSet(absentUSet);
	// std::cout << "\n\n";

	if (!correctUSet.empty()) {
		if (!presentUSet.empty()) {
			uset_intersection(correctUSet, presentUSet);
		}
		if (!absentUSet.empty()) {
			uset_difference(correctUSet, absentUSet);
		}
		// printSet(correctUSet);
		return correctUSet;
	} else if (!presentUSet.empty()) {
		if (!absentUSet.empty()) {
			uset_difference(presentUSet, absentUSet);
		}
		// printSet(presentUSet);
		return presentUSet;
	} else {
		correctUSet = invertedIndex->words();
		uset_difference(correctUSet, absentUSet);
		// printSet(result);
		return correctUSet;
	}
}
