#pragma once
#include "player.h"
class Individual
{
private:
	Player& player;
	Player* playerPointer;
	int numPlayedCompetitions;
	bool currentlyPlaying;
	float score;
public:
	Individual(Player* player);

	/*
	Register a played competition by adding 1 to 'numPlayedCompetitions'. 
	*/
	void addPlayedCompetition();

	/*
	Getter functions for the number of played competitions, the pointer of the corresponding player, 
	and the score of the individual.
	*/
	int getNumPlayedCompetitions();
	Player* getPlayer();
	float getScore();

	/*
	Update the score of the individual by adding the average of the player earnings after multiple rounds.
	*/
	void updateScore();

	/*
	Function that computes the cross over operation in the genetic algorithm.
	@param strategy:		The strategy vector that the individual is crossing with.
	@param idx:			The cross over index.
	*/
	void crossOver(std::vector<float> strategy, int idx);

	/*
	Function that computes the mutation operation in the genetic algorithm.
	@param noise:	Vector that contains the noise values to be added to the strategy vector.
	@param mask:	A vector of 0s and 1s that indicate the positions of the vector that will be mutated.
	*/
	void mutateStrategyElementByIndexVector(std::vector<float> noise, std::vector<int> mask);
	
	/*
	Functions that sets the current state of the individual.
	*/
	void beginPlaying();
	void endPlaying();
	bool isCurrentlyPlaying();

	/*
	* Function that resets the score, the number of played competitions and the player earnings.
	*/
	void reset();

	/*
	Function that clones an individual for the selection block.
	@returns:	A pointer to the new individual.
	*/
	Individual* clone();
};