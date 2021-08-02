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

/*
InputAgent: Agent that receives commands from the user.
*/
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
	bool parallelize;
public:
	LinearAgent(int thetaSize, bool parallelize=true);
	LinearAgent(std::vector<float> theta, bool parallelize = true);

	/*
	Make a decision in the betting round, given the current state of the game and the minimum and maximum value to raise.
	@return:	A Decision object that contains the action (Fold/Call/Raise) and the raise amount, which is set to 0
				in the case of Fold or Call.
	*/
	Decision makeDecision(int gameStateIdx, State& state, int minRaise, int maxRaise);

	/*
	Functions that print and get the strategy vector.
		*/
	void printTheta();
	std::vector<float> getTheta();

	/*
	Assign the values of the strategy vector after a certain position.
	@param strategy:		Vector of the new strategy from which the values are copied.
	@param idx:				Index of the starting point in the vector, in which the values of the strategy vector are changed.
	*/
	void assignStrategy(std::vector<float> strategy, int idx);

	/*
	Adds gaussian noise to some positions of the strategy vector. These positions are given by a mask.
	@param noise:	The noise vector to be added to the strategy vector
	@param mask:	The mask vector that contains 1's at the positions to be mutated, and 0's otherwise.
	*/
	void mutateStrategyElementByIndexVector(std::vector<float> noise, std::vector<int> mask);
};
