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
	float stack;
	Hand& hand{ Hand() };
	float lastBet{ 0.0f };
	Agent& decisionAgent;
public:
	Player(float startingStack, Agent& decisionAgent);
	void addCardToHand(std::pair<Suit, Value> card, int position);
	void resetHand();
};
