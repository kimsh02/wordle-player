#include "invertedindex.hpp"

#include <iostream>
#include <string>
#include <vector>

#include "doclen.hpp"

InvertedIndex::InvertedIndex(const std::vector<std::string> &d)
	: docs{ d }
{
	makeIndices();
	printIndices();
	makeInvertedIndex();
	printInvertedIndex();
}

void InvertedIndex::makeIndices(void)
{
	indices.reserve(MAX_INDICES);
	for (const auto &doc : docs) {
		for (std::size_t i = 0; i < DOC_LEN; i++) {
			indices.emplace(std::string{
				doc.at(i), static_cast<char>(i + '0') });
		}
		if (indices.size() == 130) {
			return;
		}
	}
}

void InvertedIndex::printIndices(void) const
{
	for (const auto &i : indices) {
		std::cout << i << "\n";
	}
	std::cout << indices.size() << "\n";
}

void InvertedIndex::makeInvertedIndex(void)
{
	for (const auto &i : indices) {
		char	    letter = i.at(0);
		std::size_t pos	   = i.at(1) - '0';
		for (const auto &doc : docs) {
			if (doc.at(pos) == letter) {
				invertedIndex[i].push_back(doc);
			}
		}
	}
}

void InvertedIndex::printInvertedIndex(void) const
{
	for (const auto &[key, value] : invertedIndex) {
		std::cout << "{ " << key << " }: [";

		for (std::size_t i = 0; i < value.size(); i++) {
			if (i == value.size() - 1) {
				std::cout << value.at(i) << "]\n";
				break;
			}
			std::cout << value.at(i) << ", ";
		}
	}
}
