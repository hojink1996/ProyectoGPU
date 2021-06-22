#pragma once
#include <array>
#include "card.h"

class Hand
{
private:
	std::array<std::pair<Suit, Value>, 2> currentHand;
public:
	Hand();
	void addCardToHand(std::pair<Suit, Value> card, int position);
	std::array<std::pair<Suit, Value>, 2> getHand();
};

