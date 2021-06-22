#include "deck.h"
#include "card.h"
#include <array>
#include <random>
#include <algorithm>

Deck::Deck(std::array<int, 52> cards)
{
	this->currentPosition = 0;
	this->cards = cards;
	this->cardOrder = std::array<std::pair<Suit, Value>, 52>();
	for (int index = 0; index < 52; ++index)
		this->cardOrder[index] = std::make_pair(static_cast<Suit>(index % 13), static_cast<Value>(index / 13));
}

Deck::Deck()
{
	this->currentPosition = 0;
	this->cards.fill(1);
	this->cardOrder = std::array<std::pair<Suit, Value>, 52>();
	for (int index = 0; index < 52; ++index)
		this->cardOrder[index] = std::make_pair(static_cast<Suit>(index % 13), static_cast<Value>(index / 13));
}

std::pair<Suit, Value> Deck::drawCard()
{
	if (this->currentPosition < 52 && this->cards[this->currentPosition])
	{
		this->cards[this->currentPosition] = 0;
		return this->cardOrder[this->currentPosition++];
	}
	else
		return std::make_pair(Suit::Invalid, Value::Invalid);
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
