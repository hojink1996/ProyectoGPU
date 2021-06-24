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
public:
	virtual Decision makeDecision(float maxBet) = 0;
};

class RandomAgent : public Agent
{
private:
public:
	RandomAgent();
	Decision makeDecision(float maxBet);
};
