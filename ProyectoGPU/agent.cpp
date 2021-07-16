#include <random>
#include <iostream>
#include "agent.h"
#include "decision.h"

using namespace std;


void LinearAgent::printTheta()
{
	cout << "Theta:" << endl;
	for (int i=0; i<this->thetaSize; i++)
	{
		cout << this->theta[i] << endl;
	}
}

float* LinearAgent::getTheta()
{
	return this->theta;
}

int LinearAgent::getThetaSize()
{
	return this->thetaSize;
}


RandomAgent::RandomAgent()
{
	
}

Decision RandomAgent::makeDecision(float* state, float maxBet, float minBet)
{
	Play play = static_cast<Play>(std::rand() % 3);
	float bet = float(rand()) / float((RAND_MAX)) * maxBet;
	Decision decision{ play, bet };

	return decision;
}

float* RandomAgent::getTheta()
{
	float theta[1];
	return theta;
}

int RandomAgent::getThetaSize()
{
	return 0;
}

LinearAgent::LinearAgent(int thetaSize)
{
	this->thetaSize = thetaSize;
	this->theta = new float[thetaSize]();
	for (int i = 0; i < thetaSize; i++)
	{
		// Fill with random values between -1 and 1
		this->theta[i] = (float)rand() / RAND_MAX * 2 - 1;
	}
}

bool LinearAgent::compute_fold(float* state)
{
	int size = sizeof(state) / sizeof(state[0]);
	float result = 0.0f;
	for (int i = 0; i < size; i++)
	{
		result += this->theta[i] * state[i];
	}
	return result < 0.0;
}

float LinearAgent::compute_amount(float* state)
{
	int size = sizeof(state) / sizeof(state[0]);
	float result = 0.0f;
	for (int i = size; i < size * 2; i++)
	{
		result += this->theta[i] * state[i];
	}
	return result;
}

Decision LinearAgent::makeDecision(float* state, float maxBet, float minBet)
{
	// Decide whether to fold or to keep playing
	bool fold = this->compute_fold(state);

	Play play;
	float amount;

	if (fold)
	{
		play = Play::Fold;
		amount = 0.0f;
	}
	else
	{
		amount = max(min(this->compute_amount(state), maxBet), minBet);
		play = static_cast<Play>((int)(amount > minBet) + 1); // Call if bet amount is equal to minBet, Raise otherwise
	}
	Decision decision{ play, amount };

	return decision;
}
