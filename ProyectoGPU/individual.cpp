#include <numeric>
#include "individual.h"
#include <mutex>
#include <thread>

std::mutex mtx;

Individual::Individual(Player* player) : player(*player)
{
	this->playerPointer = player;
	this->numPlayedCompetitions = 0;
	this->score = 0.0f;
	this->currentlyPlaying = false;
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
	std::vector<float> earnings = this->player.getPlayerEarnings();
	
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
	std::vector<float> theta = this->player.getAgent().getTheta();
	LinearAgent* newAgent = new LinearAgent(theta);
	Individual* ind = new Individual(new Player(*newAgent));
	return ind;
}

void Individual::beginPlaying()
{
	mtx.lock();
	this->currentlyPlaying = true;
	mtx.unlock();
}

void Individual::endPlaying()
{
	mtx.lock();
	this->currentlyPlaying = false;
	mtx.unlock();
}

bool Individual::isCurrentlyPlaying()
{
	mtx.lock();
	bool isCurrentlyPlaying = this->currentlyPlaying;
	mtx.unlock();

	return isCurrentlyPlaying;
}
