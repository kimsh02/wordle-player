#include "invertedindex.hpp"

#include <iostream>
#include <string>
#include <unordered_map>

#include "doclen.hpp"

InvertedIndex::InvertedIndex(const std::unordered_set<std::string> &docs)
	: docs{ docs }
	, indexPair{ makeIndexPair() }
{
	// printInvertedIndex();
}

std::string InvertedIndex::key(char letter, std::size_t pos) const
{
	return std::string{ letter, static_cast<char>(pos + '0') };
}

const std::unordered_set<std::string> &InvertedIndex::at(char	     letter,
							 std::size_t pos) const
{
	return indexPair.first.at(key(letter, pos));
}

std::size_t InvertedIndex::frequency(char letter, std::size_t pos) const
{
	return indexPair.second.at(key(letter, pos));
}

std::unordered_set<std::string> InvertedIndex::makeKeys(void) const
{
	std::unordered_set<std::string> keys{};
	keys.reserve(MAX_KEYS);
	for (const auto &doc : docs) {
		for (std::size_t i = 0; i < DOC_LEN; i++) {
			keys.emplace(key(doc[i], i));
		}
		if (keys.size() == 130) {
			break;
		}
	}
	return keys;
}

// void InvertedIndex::printIndices(void) const
// {
// 	for (const auto &i : indices) {
// 		std::cout << i << "\n";
// 	}
// 	std::cout << indices.size() << "\n";
// }

IndexPair InvertedIndex::makeIndexPair(void) const
{
	auto	  keys{ makeKeys() };
	IndexPair indexPair{};
	indexPair.first.reserve(keys.size());
	indexPair.second.reserve(keys.size());
	for (const auto &i : keys) {
		char	    letter = i[0];
		std::size_t pos	   = i[1] - '0';
		for (const auto &doc : docs) {
			if (doc[pos] == letter) {
				indexPair.first[i].insert(doc);
			}
		}
		indexPair.second[i] = indexPair.first[i].size();
	}
	return indexPair;
}

void InvertedIndex::printInvertedIndex(void) const
{
	std::size_t ctr = 0;
	for (const auto &[key, value] : indexPair.first) {
		std::cout << "{ " << key << " }: [";
		std::size_t i = 0;
		ctr += value.size();
		for (auto it = value.begin(); it != value.end(); it++, i++) {
			if (i == value.size() - 1) {
				std::cout << *it << "]\n";
				break;
			}
			std::cout << *it << ", ";
		}
	}
	std::cout << ctr << "\n";
}

const IndexPair &InvertedIndex::get(void) const
{
	return indexPair;
}

const std::unordered_set<std::string> &InvertedIndex::words(void) const
{
	return docs;
}
