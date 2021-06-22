#include "card.h"
#include "player.h"

Player::Player(float startingStack)
{
	this->stack = startingStack;
}

void Player::addCardToHand(std::pair<Suit, Value> card, int position)
{
	this->hand.addCardToHand(card, position);
}
