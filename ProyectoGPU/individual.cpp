#include <numeric>
#include "individual.h"

Individual::Individual(Player* player) : player(*player)
{
	this->playerPointer = player;
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

void Individual::crossOver(std::vector<float> strategy, int idx)
{
	this->player.assignStrategy(strategy, idx);
}

Player* Individual::getPlayer()
{
	return this->playerPointer;
}

void Individual::mutateStrategyElementByIndexVector(std::vector<int> indexesToBeMutated)
{
	this->player.mutateStrategyElementByIndexVector(indexesToBeMutated);
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

void Individual::reset()
{
	this->score = 0.0f;
	this->numPlayedCompetitions = 0;
	this->player.resetEarnings();
}