#include "deck.h"
#include <array>
#include <algorithm>

Deck::Deck(std::array<int, 52> cards)
{
	this->cards = cards;
}

Deck::Deck()
{
	this->cards.fill(1);
}

