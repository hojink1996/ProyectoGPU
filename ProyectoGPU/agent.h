#pragma once
#include <vector>
#include "decision.h"
#include "state.h"

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
	virtual Decision makeDecision(int gameStateIdx, State& state, int minRaise, int maxRaise) = 0;
	// virtual Decision makeDecision(int minimumBet, int maximumBet) = 0;
	virtual std::vector<float> getTheta() = 0;
	virtual void assignStrategy(std::vector<float> strategy, int idx) = 0;
	virtual void mutateStrategyElementByIndexVector(std::vector<float> noise, std::vector<int> mask) = 0;
};

/*
RandomAgent: Agent that makes decision randomly.
*/
class RandomAgent : public Agent
{
private:
public:
	RandomAgent();
	Decision makeDecision(int gameStateIdx, State& state, int minRaise, int maxRaise);
	std::vector<float> getTheta();
	void assignStrategy(std::vector<float> strategy, int idx);
	void mutateStrategyElementByIndexVector(std::vector<float> noise, std::vector<int> mask);
};

class InputAgent : public Agent
{
public:
	InputAgent();
	Decision makeDecision(int gameStateIdx, State& state, int minRaise, int maxRaise);
	std::vector<float> getTheta();
	void assignStrategy(std::vector<float> strategy, int idx);
	void mutateStrategyElementByIndexVector(std::vector<float> noise, std::vector<int> mask);
};

/*
LinearAgent: Agent that makes decision given a linear combination with its parameters.
*/
class LinearAgent : public Agent
{
private:
	std::vector<float> theta = {};
	float computeAmount(int gameStateIdx, State& state);
public:
	LinearAgent(int thetaSize);
	LinearAgent(std::vector<float> theta);
	Decision makeDecision(int gameStateIdx, State& state, int minRaise, int maxRaise);

	void printTheta();
	std::vector<float> getTheta();
	void assignStrategy(std::vector<float> strategy, int idx);
	void mutateStrategyElementByIndexVector(std::vector<float> noise, std::vector<int> mask);
};
