#pragma once
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
	float theta_action[10];
	float theta_amount[10];
public:
	virtual Decision makeDecision(float* state, float maxBet, float minBet) = 0;
	void initialize_theta();
	void print_theta();
	float* getTheta();
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
};

/*
LinearAgent: Agent that makes decision given a linear combination with its parameters.
*/
class LinearAgent : public Agent
{
private:
	bool compute_fold(float* state);
	float compute_amount(float* state);
public:
	LinearAgent();
	Decision makeDecision(float* state, float maxBet, float minBet);
};
