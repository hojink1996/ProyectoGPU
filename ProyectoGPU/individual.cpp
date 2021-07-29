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

void Individual::mutateStrategyElementByIndexVector(std::vector<float> noise, std::vector<int> mask)
{
	this->player.mutateStrategyElementByIndexVector(noise, mask);
}

float Individual::getScore()
{
	return this->score;
}

void Individual::updateScore()
{
	std::vector<int> earnings = this->player.getPlayerEarnings();
	
	// Compute mean of earnings and add to score
	if (earnings.size() != 0)
		this->score += std::accumulate(earnings.begin(), earnings.end(), 0.0) / earnings.size();
}

void Individual::reset()
{
	this->score = 0.0f;
	this->numPlayedCompetitions = 0;
	this->player.resetEarnings();
}

Individual* Individual::clone()
{
	return new Individual(&this->player);
}