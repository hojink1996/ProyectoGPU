#include "deck.h"
#include "card.h"
#include <array>
#include <random>
#include <algorithm>

Deck::Deck(std::array<int, 52> cards)
{
	this->currentPosition = 0;
	this->cards = cards;
	this->cardOrder = std::array<Card, 52>();
	for (int index = 0; index < 52; ++index)
		this->cardOrder[index] = std::make_pair(static_cast<Suit>(index / 13), static_cast<Value>(index % 13));
}

Deck::Deck()
{
	this->currentPosition = 0;
	this->cards.fill(1);
	this->cardOrder = std::array<Card, 52> {};
	for (int index = 0; index < 52; ++index)
		this->cardOrder[index] = std::make_pair(static_cast<Suit>(index / 13), static_cast<Value>(index % 13));
}

Card Deck::drawCard()
{
	if (this->currentPosition < 52 && this->cards[this->currentPosition])
	{
		this->cards[this->currentPosition] = 0;
		return this->cardOrder[this->currentPosition++];
	}
	else
		return invalidCard;
}

void Deck::shuffle()
{
	random_shuffle(std::begin(this->cardOrder), std::end(this->cardOrder));
}

void Deck::reset()
{
	this->shuffle();
	this->cards.fill(1);
	this->currentPosition = 0;
}
