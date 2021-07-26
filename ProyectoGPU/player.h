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

	std::vector<int> playerEarnings;
public:
	Player(int startingStack, Agent& decisionAgent);
	void addCardToHand(Card card, int position);
	void resetHand();
	Hand getHand();

	/*
	This function is used for the player to make the decision of what play to make next.

	@param minimumBet:		The minimum amount of money that the player can add to the current bet value.
	@param currentBetValue:	The current amount of money that the bet is standing at. Then, the maximum amount of money
							that the player can bet at the given moment is given by (this->stack - currentBetValue), which
							is equivalent to betting the rest of the remaining money.
	@return:				A decision structure that contains the information of the decision made by the player.
	*/
	//Decision makeDecision(int minimumBet, int currentBetValue);
	Decision decide(int gameStateIdx, std::vector<float> state, int minBet, int currentBetValue);

	/*
	Function used to add the player earnings during a round. The function takes the amount of money earned and the amount of money
	remaining and compares the value with the amount of money at the begining of the round to compute the actual earnings
	of the player during the round.
	
	@param earnings:		The amount of money earned by the player during the round (0 if the player lost the money).
	@param originalStack:	The original amount of money that the player had before the begining of the round.
	*/
	void addPlayerEarnings(int earnings, int originalStack);

	/*
	Function used to decrease the stack size of the player when they bet.
	The function simply takes the player's stack (this->stack) and updates it by decreasing it by 'decreaseValue'.

	@param decreaseValue:	The amount in which the stack is decreased.
	*/
	void decreaseStackSize(int decreaseValue);

	/*
	Functions used to get the history of player earnings (or losses, which are represented as "negative earnings")
	once the playing rounds are over.
	
	@return:		A vector of length 'n', with 'n' being the number of rounds played by the player.
					Each element in the vector represents the player's earning during one of these rounds.
	*/
	std::vector<int> getPlayerEarnings();

	void bet(int amount);
	void setPlayerStack(int newStack);

	std::vector<float> getStrategy();
	Agent& getAgent();
	void assignStrategy(std::vector<float> strategy, int idx);
	void mutateStrategyElementByIndexVector(std::vector<int> indexesToBeMutated);

	int getStack();
};
