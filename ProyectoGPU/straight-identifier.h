#pragma once
#include <array>
#include <vector>
#include "card.h"

class StraightIdentifier
{
private:
	int maxLength;
	Value nextValue;
	Value currentValue;
	void reset();
public:
	StraightIdentifier();
	bool hasRoyalStraight(const std::vector<Value>& filteredBySuitAndOrdered, Value& highCard);
	bool hasStraightFlush(const std::vector<Value>& filteredBySuitAndOrdered, Value& highCard);
	bool hasStraight(const std::vector<Value>& orderedByValue, Value& highCard);
};
