#include <random>
#include "agent.h"
#include "decision.h"

RandomAgent::RandomAgent()
{
	
}

Decision RandomAgent::makeDecision(float maxBet)
{
	Play play = static_cast<Play>(std::rand() % 3);
	float bet = float(rand()) / float((RAND_MAX)) * maxBet;
	Decision decision{ play, bet };

	return decision;
}
