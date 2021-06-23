#include "card.h"
#include "player.h"

Player::Player(float startingStack, Agent& decisionAgent) : decisionAgent(decisionAgent)
{
	this->stack = startingStack;
}

void Player::addCardToHand(std::pair<Suit, Value> card, int position)
{
	this->hand.addCardToHand(card, position);
}

void Player::resetHand()
{
	this->hand.resetHand();
}
