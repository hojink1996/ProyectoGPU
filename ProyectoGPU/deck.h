#pragma once
#include <array>
#include "card.h"

/*
The Deck Class is used to represent a deck of playing cards.
The deck has a set of 52 playing cards, given by the possible combinations of
suits and values specified in the "card.h" file.
It also provides functionality to keep track of the available cards in the deck,
shuffling decks and resetting the deck (which shuffles the deck and returns all
of the cards to the deck).
*/
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
