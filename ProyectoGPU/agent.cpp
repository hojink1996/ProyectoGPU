#include <random>
#include <iostream>
#include <cassert>
#include "agent.h"
#include "decision.h"


using namespace std;


void LinearAgent::printTheta()
{
	cout << "Theta:" << endl;
	for (int i=0; i < this->theta.size(); i++)
	{
		cout << this->theta[i] << endl;
	}
}

std::vector<float> LinearAgent::getTheta()
{
	return this->theta;
}

void LinearAgent::assignStrategy(std::vector<float> strategy, int idx)
{
	for (int i = idx; i < this->theta.size(); i++)
	{
		this->theta[i] = strategy[i];
	}
}


RandomAgent::RandomAgent()
{
	
}

void RandomAgent::mutateStrategyElementByIndexVector(std::vector<int> indexesToBeMutated)
{
}

Decision RandomAgent::makeDecision(std::vector<float> state, float minRaise, float maxRaise)
{
	assert(state.size() > 0);
	assert(minRaise >= 0);

	Play play = static_cast<Play>(std::rand() % 3);
	float bet = max(float(rand()) / float((RAND_MAX)), minRaise);
	Decision decision{ play, bet };

	return decision;
}

std::vector<float> RandomAgent::getTheta()
{
	return std::vector<float>();
}

void RandomAgent::assignStrategy(std::vector<float> strategy, int idx)
{
}

LinearAgent::LinearAgent(int thetaSize)
{
	assert(thetaSize > 0);
	for (int i = 0; i < thetaSize * 4; i++) // First 1/3 is to compute Fold, second 2/3 for Call, and rest for amount
	{
		// Fill with random values between -1 and 1
		this->theta.push_back((float)rand() / RAND_MAX * 2 - 1);
	}
}


float LinearAgent::computeAmount(std::vector<float> state)
{
	assert(state.size() * 4 == this->theta.size());

	float result = 0.0f;
	for (int i = state.size() * 3; i < state.size() * 4; i++)
	{
		result += this->theta[i] * state[i % state.size()];
	}
	return result;
}

Decision LinearAgent::makeDecision(std::vector<float> state, float minRaise, float maxRaise)
{
	assert(state.size() * 4 == this->theta.size());
	assert(minRaise >= 0);

	Play play;
	float raiseAmount;

	// If the amount in stack is not enough
	if (maxRaise < 0.0f)
	{
		Decision decision{ Play::Fold, 0.0f };
		return decision;
	}

	// Compute linear combination
	std::vector<float> result = { 0.0f, 0.0f, 0.0f };  // fold, call, raise
	for (int i = 0; i < state.size(); i++)
	{
		result[0] += this->theta[i] * state[i];
		result[1] += this->theta[state.size() + i] * state[i];
		result[2] += this->theta[state.size() * 2 + i] * state[i];
	}

	// Compute softmax
	float normalizingConstant = exp(result[0]) + exp(result[1]) + exp(result[2]);
	for (int i = 0 ; i < 3; i++)
	{
		result[i] = result[i] / normalizingConstant;
	}

	// Get argmax
	int maxIdx = std::distance(result.begin(), std::max_element(result.begin(), result.end()));

	if (maxIdx == 2)  // Raise
	{
		if (maxRaise == 0.0f)  // If agent decided to Raise, but maxRaise is zero, it then decides to Call
		{
			play = Play::Call;
			raiseAmount = 0.0f;
		}
		else 
		{
			play = Play::Raise;
			raiseAmount = min(max(this->computeAmount(state), minRaise), maxRaise);;
		}
	}
	else  // Fold or Call
	{
		play = static_cast<Play>(maxIdx);
		raiseAmount = 0.0f;
	}

	Decision decision{ play, raiseAmount };

	return decision;
}

void LinearAgent::mutateStrategyElementByIndexVector(std::vector<int> indexesToBeMutated)
{
	for (int i : indexesToBeMutated)
	{
		this->theta[i] = (float)rand() / RAND_MAX * 2 - 1;
	}
}
