#include "hand.h"

Hand::Hand()
{
	this->currentHand = { invalidCard, invalidCard };
}

void Hand::addCardToHand(Card card, int position)
{
	this->currentHand[position] = card;
}

std::array<Card, 2> Hand::getHand()
{
	return this->currentHand;
}

void Hand::resetHand()
{
	this->currentHand = { invalidCard, invalidCard };
}