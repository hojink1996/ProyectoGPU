#include "straight-identifier.h"

StraightIdentifier::StraightIdentifier()
{
	this->maxLength = 0;
	this->nextValue = Value::Invalid;
}

void StraightIdentifier::reset()
{
	this->maxLength = 0;
	this->nextValue = Value::Invalid;
}

bool StraightIdentifier::hasRoyalStraight(const std::vector<Value>& filteredBySuitAndOrdered)
{
	// Loop over all the values with the same suit
	for (int currentCard = 0; currentCard < filteredBySuitAndOrdered.size(); ++currentCard)
	{
		// For a royal straight the first card must be an ace
		if (currentCard == 0)
		{
			if (filteredBySuitAndOrdered[currentCard] != Value::Ace)
				return false;
			this->nextValue = Value::Ten;
			++this->maxLength;
		}
		else
		{
			Value currentValue = filteredBySuitAndOrdered[currentCard];
			// If we don't have the next card, let's check if it's in a different position (you could have Ace, Five, Ten)
			if (currentValue != this->nextValue)
				continue;
			this->nextValue = static_cast<Value>((static_cast<int>(currentValue) + 1) % 13);
			++this->maxLength;
		}
	}
	int maxLength = this->maxLength;
	this->reset();
	return maxLength >= 5;
}
