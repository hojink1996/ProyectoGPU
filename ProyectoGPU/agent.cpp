#include <random>
#include <iostream>
#include <cassert>
#include "agent.h"
#include "decision.h"
#include "operations.cuh"


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

void RandomAgent::mutateStrategyElementByIndexVector(std::vector<float> noise, std::vector<int> mask)
{
}

Decision RandomAgent::makeDecision(int gameStateIdx, std::vector<float> state, float minRaise, float maxRaise)
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
	for (int i = 0; i < thetaSize * 16; i++) // First 1/3 is to compute Fold, second 2/3 for Call, and rest for amount
	{
		// Fill with random values between -1 and 1
		this->theta.push_back((float)rand() / RAND_MAX * 2 - 1);
	}
}


float LinearAgent::computeAmount(int gameStateIdx, std::vector<float> state)
{
	assert(state.size() * 16 == this->theta.size());

	int offset = gameStateIdx * state.size() * 4;  // Offset to operate with the theta corresponding to the current game state
	float result = 0.0f;
	for (int i = state.size() * 3; i < state.size() * 4; i++)
	{
		result += this->theta[offset + i] * state[i % state.size()];
	}
	return result;
}

Decision LinearAgent::makeDecision(int gameStateIdx, std::vector<float> state, float minRaise, float maxRaise)
{
	assert(state.size() * 16 == this->theta.size());
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
	int offset = gameStateIdx * state.size();  // Offset to operate with the theta corresponding to the current game state
	std::vector<float> result(4, 0);  // fold, call, raise

	result[0] = CudaFunctions::dotProduct(&this->theta[offset], &state[0], state.size());
	result[1] = CudaFunctions::dotProduct(&this->theta[offset + state.size()], &state[0], state.size());
	result[2] = CudaFunctions::dotProduct(&this->theta[offset + state.size() * 2], &state[0], state.size());

	
	// Compute softmax

	/*
	float normalizingConstant = exp(result[0]) + exp(result[1]) + exp(result[2]);
	for (int i = 0 ; i < 3; i++)
	{
		result[i] = exp(result[i]) / normalizingConstant;
	}
	*/

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
			raiseAmount = min(max(this->computeAmount(gameStateIdx, state), minRaise), maxRaise);;
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

void LinearAgent::mutateStrategyElementByIndexVector(std::vector<float> noise, std::vector<int> mask)
{
	CudaFunctions::maskedAdd(&this->theta[0], &noise[0], &mask[0], this->theta.size(), &this->theta[0]);
}
