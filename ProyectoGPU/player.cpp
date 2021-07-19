#include "card.h"
#include "player.h"
#include <cassert>

Player::Player(int startingStack, Agent* decisionAgent) : decisionAgent(*decisionAgent)
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

/*
Decision Player::makeDecision(int minimumBet, int currentBetValue)
{
	return this->decisionAgent.makeDecision(minimumBet, this->stack - currentBetValue);
}
*/

Decision Player::decide(std::vector<float> state, int minBet)
{
	return this->decisionAgent.makeDecision(state, minBet);
}

void Player::bet(int amount)
{
	this->stack -= amount;
}


Agent& Player::getAgent()
{
	return this->decisionAgent;
}

std::vector<float> Player::getStrategy()
{
	return this->decisionAgent.getTheta();
}

void Player::setPlayerStack(int newStack)
{
	this->stack = newStack;
}


void Player::decreaseStackSize(int decreaseValue)
{
	this->stack -= decreaseValue;
	assert(this->stack >= 0);
}

void Player::addPlayerEarnings(int earnings, int startingStack)
{
	this->playerEarnings.push_back(earnings + this->stack - startingStack);
}

std::vector<int> Player::getPlayerEarnings()
{
	return this->playerEarnings;
}
