#include "invertedindex.hpp"

#include <iostream>
#include <string>
#include <vector>

InvertedIndex::InvertedIndex(const std::vector<std::string> &d)
	: docs{ d }
{
	makeIndices();
	printIndices();
}

void InvertedIndex::makeIndices(void)
{
	indices.reserve(MAX_INDICES);
	for (const auto &doc : docs) {
		for (std::size_t i = 0; i < DOC_LENGTH; i++) {
			std::array<char, BUFFER_SIZE> buf = {
				doc[i], static_cast<char>(i + '0')
			};
			std::string index{ buf.data(), BUFFER_SIZE };
			indices.emplace(std::move(index));
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
}

void InvertedIndex::printInvertedIndex(void) const
{
}
