#include "invertedindex.hpp"

#include <iostream>
#include <string>
#include <unordered_map>

#include "doclen.hpp"

InvertedIndex::InvertedIndex(const std::unordered_set<std::string> &docs)
	: docs{ docs }
	, indexPair{ makeIndexPair() }
{
	printInvertedIndex();
}

std::unordered_set<std::string> InvertedIndex::makeIndices(void)
{
	std::unordered_set<std::string> indices{};
	indices.reserve(MAX_INDICES);
	for (const auto &doc : docs) {
		for (std::size_t i = 0; i < DOC_LEN; i++) {
			indices.emplace(std::string{
				doc.at(i), static_cast<char>(i + '0') });
		}
		if (indices.size() == 130) {
			break;
		}
	}
	return indices;
}

// void InvertedIndex::printIndices(void) const
// {
// 	for (const auto &i : indices) {
// 		std::cout << i << "\n";
// 	}
// 	std::cout << indices.size() << "\n";
// }

IndexPair InvertedIndex::makeIndexPair(void)
{
	auto	  indices{ makeIndices() };
	IndexPair indexPair{};
	indexPair.first.reserve(indices.size());
	indexPair.second.reserve(indices.size());
	for (const auto &i : indices) {
		char	    letter = i.at(0);
		std::size_t pos	   = i.at(1) - '0';
		for (const auto &doc : docs) {
			if (doc.at(pos) == letter) {
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
