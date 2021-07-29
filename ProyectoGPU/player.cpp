#include "card.h"
#include "player.h"
#include <cassert>

Player::Player(Agent& decisionAgent) : decisionAgent(decisionAgent)
{
	this->stack = 0;
	this->playerEarnings = {};
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

Decision Player::decide(int gameStateIdx, State& state, int minBet, int currentBetValue)
{
	Decision decision = this->decisionAgent.makeDecision(gameStateIdx, state, minBet, this->stack - currentBetValue);
	return decision;
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


void Player::assignStrategy(std::vector<float> strategy, int idx)
{
	this->decisionAgent.assignStrategy(strategy, idx);
}

void Player::mutateStrategyElementByIndexVector(std::vector<float> noise, std::vector<int> mask)
{
	this->decisionAgent.mutateStrategyElementByIndexVector(noise, mask);
}

int Player::getStack()
{
	return this->stack;
}

void Player::resetEarnings()
{
	this->playerEarnings.clear();
}

void Player::assignStack(int stack)
{
	this->stack = stack;
}