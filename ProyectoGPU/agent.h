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
	virtual Decision makeDecision(std::vector<float> state, float minRaise) = 0;
	// virtual Decision makeDecision(int minimumBet, int maximumBet) = 0;
	virtual std::vector<float> getTheta();
};

/*
RandomAgent: Agent that makes decision randomly.
*/
class RandomAgent : public Agent
{
private:
public:
	RandomAgent();
	Decision makeDecision(std::vector<float> state, float minRaise);
	std::vector<float> getTheta();
};

/*
LinearAgent: Agent that makes decision given a linear combination with its parameters.
*/
class LinearAgent : public Agent
{
private:
	int thetaSize;
	std::vector<float> theta;

	bool computeFold(std::vector<float> state);
	bool computeCall(std::vector<float> state);
	float computeAmount(std::vector<float> state);
public:
	LinearAgent(int thetaSize);
	Decision makeDecision(std::vector<float> state, float minRaise);

	void printTheta();
	std::vector<float> getTheta();
};
