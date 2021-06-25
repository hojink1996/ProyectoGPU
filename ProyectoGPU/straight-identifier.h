#pragma once
#include <array>
#include <vector>
#include "card.h"

class StraightIdentifier
{
private:
	int maxLength;
	Value nextValue;
	void reset();
public:
	StraightIdentifier();
	bool hasRoyalStraight(const std::vector<Value>& filteredBySuitAndOrdered);
	bool hasStraight(const std::vector<Value>& hand);
};