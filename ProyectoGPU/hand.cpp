#include "hand.h"

Hand::Hand()
{
	this->currentHand = { std::make_pair(Suit::Invalid, Value::Invalid),
						 std::make_pair(Suit::Invalid, Value::Invalid)};
}

void Hand::addCardToHand(std::pair<Suit, Value> card, int position)
{
	this->currentHand[position] = card;
}

std::array<std::pair<Suit, Value>, 2> Hand::getHand()
{
	return this->currentHand;
}

void Hand::resetHand()
{
	this->currentHand = { std::make_pair(Suit::Invalid, Value::Invalid),
						 std::make_pair(Suit::Invalid, Value::Invalid)};
}