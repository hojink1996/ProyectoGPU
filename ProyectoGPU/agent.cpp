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

InputAgent::InputAgent()
{

}

Decision InputAgent::makeDecision(int gameStateIdx, State& state, int minRaise, int maxRaise)
{
	bool definedPlay = false;
	int raiseAmount{ 0 };
	char play;
	Play currentPlay;
	while (!definedPlay)
	{
		std::cout << "Input the play you want to make (f for Fold, c for Call, r for Raise): ";
		std::cin >> play;

		switch (play)
		{
		case 'f':
			currentPlay = Play::Fold;
			definedPlay = true;
			break;
		case 'c':
			currentPlay = Play::Call;
			definedPlay = true;
			break;
		case 'r':
			currentPlay = Play::Raise;
			definedPlay = true;
			break;
		default:
			break;
		}
	}
	if (currentPlay == Play::Raise)
	{
		std::cout << "Input the amount you want to raise. (Min=" << minRaise << ", Max=" << maxRaise << "):";
		std::cin >> raiseAmount;
		if (raiseAmount < minRaise)
			raiseAmount = minRaise;
		if (raiseAmount > maxRaise)
			raiseAmount = maxRaise;
	}
	Decision outputDecision{ currentPlay, raiseAmount };
	return outputDecision;
}

std::vector<float> InputAgent::getTheta()
{
	return {};
}

void InputAgent::assignStrategy(std::vector<float> strategy, int idx)
{

}

void InputAgent::mutateStrategyElementByIndexVector(std::vector<float> noise, std::vector<int> mask)
{

}

RandomAgent::RandomAgent()
{
	
}

void RandomAgent::mutateStrategyElementByIndexVector(std::vector<float> noise, std::vector<int> mask)
{
}

Decision RandomAgent::makeDecision(int gameStateIdx, State& state, int minRaise, int maxRaise)
{
	assert(sizeof(state) > 0);
	assert(minRaise >= 0);

	Play play = static_cast<Play>(std::rand() % 3);
	float bet = max(rand() / RAND_MAX, minRaise);
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

LinearAgent::LinearAgent(std::vector<float> theta)
{
	assert(theta.size() > 0);
	std::copy(theta.begin(), theta.end(), back_inserter(this->theta));
}


float LinearAgent::computeAmount(int gameStateIdx, State& state)
{
	int stateSize = sizeof(state.values) / sizeof(state.values[0]);
	assert(stateSize * 16 == this->theta.size());

	int offset = gameStateIdx * stateSize * 4;  // Offset to operate with the theta corresponding to the current game state
	float result = 0.0f;
	for (int i = stateSize * 3; i < stateSize * 4; i++)
	{
		result += this->theta[offset + i] * state.values[i % stateSize];
	}
	return result;
}

Decision LinearAgent::makeDecision(int gameStateIdx, State& state, int minRaise, int maxRaise)
{
	int stateSize = sizeof(state.values) / sizeof(state.values[0]);
	assert(stateSize * 16 == this->theta.size());
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
	int offset = gameStateIdx * stateSize;  // Offset to operate with the theta corresponding to the current game state
	std::vector<float> result(4, 0);  // fold, call, raise
	CudaFunctions::dotProductWindow(&this->theta[offset], &state.values[0], stateSize, &result[0], result.size());

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
			raiseAmount = min(max((int)result[3], minRaise), maxRaise);;
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
