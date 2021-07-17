#pragma once
#include "hand.h"
#include "agent.h"

/*
The Player Class is used to represent a player playing the game.
The user has it's stack, hand of cards and an agent that makes the decisions
of how the player should play each hand. The class provides functionality for 
receiving cards in the game, resetting the hand and making decisions by the user.
*/
class Player
{
private:
	int stack;
	Hand& hand{ Hand() };
	float lastBet{ 0.0f };
	Agent& decisionAgent;

	int numPlayedGames;
	int numWins;

public:
	Player(int startingStack, Agent* decisionAgent);
	void addCardToHand(Card card, int position);
	void resetHand();
	Hand getHand();
	Decision makeDecision();
	void bet(int amount);
	Decision decide(float* state, int maxBet, int minBet);

	void registerWin();
	void addPlayedGame();
	int  getNumPlayedGames();
	int getNumWins();

	float* getStrategy();
	int getStrategySize();
};
