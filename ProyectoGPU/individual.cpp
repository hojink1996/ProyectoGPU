#include <numeric>
#include "individual.h"

Individual::Individual(Player& player) : player(player)
{
	this->numPlayedCompetitions = 0;
	this->score = 0.0f;
}

void Individual::addPlayedCompetition()
{
	this->numPlayedCompetitions++;
}

int Individual::getNumPlayedCompetitions()
{
	return this->numPlayedCompetitions;
}

Player& Individual::getPlayer()
{
	return this->player;
}

float Individual::getScore()
{
	return this->score;
}

void Individual::updateScore()
{
	std::vector<int> scores = this->player.getPlayerEarnings();
	
	// Compute mean of earnings and add to score
	if (scores.size() != 0)
		this->score += std::accumulate(scores.begin(), scores.end(), 0.0) / scores.size();	
}
