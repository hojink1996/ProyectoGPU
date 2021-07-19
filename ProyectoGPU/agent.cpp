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


RandomAgent::RandomAgent()
{
	
}

Decision RandomAgent::makeDecision(std::vector<float> state, float minRaise)
{
	assert(state.size() > 0);
	assert(minRaise >= 0);

	Play play = static_cast<Play>(std::rand() % 3);
	float bet = max(float(rand()) / float((RAND_MAX)), minRaise);
	Decision decision{ play, bet };

	return decision;
}

LinearAgent::LinearAgent(int thetaSize)
{
	assert(thetaSize > 0);
	for (int i = 0; i < thetaSize * 3; i++) // First 1/3 is to compute Fold, second 2/3 for Call, and rest for amount
	{
		// Fill with random values between -1 and 1
		this->theta.push_back((float)rand() / RAND_MAX * 2 - 1);
	}
}

bool LinearAgent::computeFold(std::vector<float> state)
{
	assert(state.size() * 3 == this->theta.size());

	float result = 0.0f;
	for (int i = 0; i < state.size(); i++)
	{
		result += this->theta[i] * state[i];
	}
	return result < 0.0;
}

bool LinearAgent::computeCall(std::vector<float> state)
{
	assert(state.size() * 3 == this->theta.size());

	float result = 0.0f;
	for (int i = state.size(); i < state.size() * 2; i++)
	{
		result += this->theta[i] * state[i];
	}
	return result < 0.0;
}

float LinearAgent::computeAmount(std::vector<float> state)
{
	assert(state.size() * 3 == this->theta.size());

	float result = 0.0f;
	for (int i = state.size() * 2; i < state.size() * 3; i++)
	{
		result += this->theta[i] * state[i];
	}
	return result;
}

Decision LinearAgent::makeDecision(std::vector<float> state, float minRaise)
{
	assert(state.size() * 3 == this->theta.size());
	assert(minRaise >= 0);

	Play play;
	float raiseAmount;

	// Decide whether to fold or to keep playing
	bool fold = this->computeFold(state);

	if (fold)
	{
		play = Play::Fold;
		raiseAmount = 0.0f;
	}
	else
	{
		// Decide whether to call or to raise
		bool call = this->computeCall(state);
		if (call)
		{
			play = Play::Call;
			raiseAmount = 0.0f;
		}
		else
		{
			play = Play::Raise;
			raiseAmount = max(this->computeAmount(state), minRaise);
		}
	}
	Decision decision{ play, raiseAmount };

	return decision;
}
