#pragma once
#include <vector>
#include "decision.h"

/*
The Agent Class is an Abstract Class used as a proxy for the decisions that the
player makes. It is used by each player to make a decision in different ways
(e.g. you can have a ConsoleAgent that takes the decision from a real player
from the command line, or a GeneticAlgorithmAgent that takes the decision from
a trained Genetic Algorithm).
*/
class Agent
{
private:
protected:
	
public:
	virtual Decision makeDecision(float* state, float maxBet, float minBet) = 0;
	virtual float* getTheta() = 0;
	virtual int getThetaSize() = 0;
};

/*
RandomAgent: Agent that makes decision randomly.
*/
class RandomAgent : public Agent
{
private:
public:
	RandomAgent();
	Decision makeDecision(float* state, float maxBet, float minBet);
	float* getTheta();
	int getThetaSize();
};

/*
LinearAgent: Agent that makes decision given a linear combination with its parameters.
*/
class LinearAgent : public Agent
{
private:
	int thetaSize;
	float* theta;  // first 10 is for action, last 10 is for amount

	bool compute_fold(float* state);
	float compute_amount(float* state);
public:
	LinearAgent(int thetaSize);
	Decision makeDecision(float* state, float maxBet, float minBet);

	void printTheta();
	float* getTheta();
	int getThetaSize();
};
