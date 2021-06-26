#include "straight-identifier.h"

StraightIdentifier::StraightIdentifier()
{
	this->maxLength = 0;
	this->nextValue = Value::Invalid;
	this->currentValue = Value::Invalid;
}

void StraightIdentifier::reset()
{
	this->maxLength = 0;
	this->nextValue = Value::Invalid;
	this->currentValue = Value::Invalid;
}

bool StraightIdentifier::hasStraight(const std::vector<Value>& orderedByValue)
{
	this->reset();
	bool startsWithAce = (orderedByValue[0] == Value::Ace);
	// Loop over the values and only update if we use the same card as before
	for (int currentCard = 0; currentCard < orderedByValue.size(); ++currentCard)
	{
		Value currentValue = orderedByValue[currentCard];
		if (currentValue == this->nextValue)
		{
			++this->maxLength;
			this->currentValue = currentValue;
			this->nextValue = static_cast<Value>((static_cast<int>(currentValue) + 1) % 13);
		}
		else if (currentValue == this->currentValue)
			continue;
		else
		{
			if (maxLength >= 5)
				return true;
			this->maxLength = 1;
			this->currentValue = currentValue;
			this->nextValue = static_cast<Value>((static_cast<int>(currentValue) + 1) % 13);
		}
	}
	if (startsWithAce && (this->nextValue == Value::Ace))
		++this->maxLength;
	int maxLength = this->maxLength;
	this->reset();
	return maxLength >= 5;
}

bool StraightIdentifier::hasRoyalStraight(const std::vector<Value>& filteredBySuitAndOrdered)
{
	this->reset();
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

bool StraightIdentifier::hasStraightFlush(const std::vector<Value>& filteredBySuitAndOrdered)
{
	this->reset();
	// Loop over the cards in the vector
	for (int currentCard = 0; currentCard < filteredBySuitAndOrdered.size(); ++currentCard)
	{
		// Get the current card in the vector
		Value currentValue = filteredBySuitAndOrdered[currentCard];
		if ((this->nextValue == Value::Invalid) || (this->nextValue == currentValue))
		{
			this->nextValue = static_cast<Value>((static_cast<int>(currentValue) + 1) % 13);
			++this->maxLength;
		}
		else
		{
			if (this->maxLength < 5)
			{
				this->nextValue = static_cast<Value>((static_cast<int>(currentValue) + 1) % 13);
				this->maxLength = 1;
			}
			else
				break;
		}
	}

	// Check if we got the needed length
	int maxLength = this->maxLength;
	this->reset();
	return maxLength >= 5;
}

