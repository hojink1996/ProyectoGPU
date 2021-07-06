#include <random>
#include <iostream>
#include "agent.h"
#include "decision.h"

using namespace std;

void Agent::initialize_theta() 
{
	for (int i = 0; i < 10; i++) 
	{
		// Fill with random values between -1 and 1
		this->theta_action[i] = (float)rand() / RAND_MAX * 2 - 1;
		this->theta_amount[i] = (float)rand() / RAND_MAX * 2 - 1;
	}
}

void Agent::print_theta()
{
	cout << "Theta action:" << endl;
	for (float value: this->theta_action)
	{
		cout << value << endl;
	}
	cout << "Theta amount:" << endl;
	for (float value : this->theta_amount)
	{
		cout << value << endl;
	}
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

LinearAgent::LinearAgent()
{

}


bool LinearAgent::compute_fold(float* state)
{
	int size = sizeof(state) / sizeof(state[0]);
	float result = 0.0f;
	for (int i = 0; i < size; i++)
	{
		result += this->theta_action[i] * state[i];
	}
	return result < 0.0;
}

float LinearAgent::compute_amount(float* state)
{
	int size = sizeof(state) / sizeof(state[0]);
	float result = 0.0f;
	for (int i = 0; i < size; i++)
	{
		result += this->theta_action[i] * state[i];
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
