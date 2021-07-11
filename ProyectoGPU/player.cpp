#include "card.h"
#include "player.h"

Player::Player(int startingStack, Agent& decisionAgent) : decisionAgent(decisionAgent)
{
	this->stack = startingStack;
}

void Player::addCardToHand(Card card, int position)
{
	this->hand.addCardToHand(card, position);
}

void Player::resetHand()
{
	this->hand.resetHand();
}

Hand Player::getHand()
{
	return this->hand;
}

Decision Player::decide(float* state, int maxBet, int minBet)
{
	return this->decisionAgent.makeDecision(state, maxBet, minBet);
}

void Player::bet(int amount)
{
	this->stack -= amount;
}
