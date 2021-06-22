#pragma once
#include <array>
#include "card.h"

class Deck
{
private:
	std::array<int, 52> cards;
	std::array<std::pair<Suit, Value>, 52> cardOrder;
	int currentPosition;
	void shuffle();
public:
	Deck(std::array<int, 52> cards);
	Deck();
	void reset();
	std::pair<Suit, Value> drawCard();
};
