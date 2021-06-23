#pragma once
#include <array>
#include "card.h"

/*
The Hand Class is used to represent a hand of playing cards for the user.
The hand is given by two cards (each given by a pair of suit and value). The hand
is iniatalized with Invalid values, and cards are added as they are provided
to the user.
*/
class Hand
{
private:
	std::array<std::pair<Suit, Value>, 2> currentHand;
public:
	Hand();
	void addCardToHand(std::pair<Suit, Value> card, int position);
	std::array<std::pair<Suit, Value>, 2> getHand();
	void resetHand();
};

