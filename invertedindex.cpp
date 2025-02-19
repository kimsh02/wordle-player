#include "invertedindex.hpp"

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
		for (size_t i = 0; i < DOC_LENGTH; i++) {
			indices.emplace(doc[i], i);
		}
		if (indices.size() == 130) {
			return;
		}
	}
}

void InvertedIndex::printIndices(void) const
{
	for (const auto &i : indices) {
		i.printIndex();
	}
}

void InvertedIndex::makeInvertedIndex(void)
{
	// for (const auto &i : indices) {}
}

void InvertedIndex::printInvertedIndex(void) const
{
}
