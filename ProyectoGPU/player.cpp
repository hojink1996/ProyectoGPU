#include "card.h"
#include "player.h"
#include <cassert>

Player::Player(int startingStack, Agent* decisionAgent) : decisionAgent(*decisionAgent)
{
	this->stack = startingStack;
	this->numPlayedGames = 0;
	this->numWins = 0;
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

Decision Player::makeDecision(int minimumBet, int currentBetValue)
{
	return this->decisionAgent.makeDecision(minimumBet, this->stack - currentBetValue);
}

Decision Player::decide(float* state, int maxBet, int minBet)
{
	return this->decisionAgent.makeDecision(state, maxBet, minBet);
}

void Player::bet(int amount)
{
	this->stack -= amount;
}


void Player::registerWin()
{
	this->numWins++;
}

void Player::addPlayedGame()
{
	this->numPlayedGames++;
}

int Player::getNumPlayedGames()
{
	return this->numPlayedGames;
}

int Player::getNumWins()
{
	return this->numWins;
}


float* Player::getStrategy()
{
	return this->decisionAgent.getTheta();
}

int Player::getStrategySize()
{
	return this->decisionAgent.getThetaSize();
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
