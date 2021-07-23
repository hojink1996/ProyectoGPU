#include <iostream>
#include <random>
#include <cassert>
#include "genetic-algorithm.h"
#include "agent.h"
#include "texas-holdem.h"
#include "individual.h"

GeneticAlgorithm::GeneticAlgorithm(int iniNumIndividuals, int numOpponents, int numGamesPerPair)
{
	assert(iniNumIndividuals > 2);
	assert(numOpponents > 0);

	this->numIndividuals = iniNumIndividuals;
	this->numOpponents = numOpponents;
	this->numGamesPerPair = numGamesPerPair;

	int startingStack = 2;	
	for (int i = 0; i < this->numIndividuals; i++) 
	{
		LinearAgent* agent = new LinearAgent(2);
		Player* player = new Player(startingStack, *agent);
		this->currentIndividuals.push_back(Individual(*player));
	}
}


void GeneticAlgorithm::compete(Individual individual1, Individual individual2)
{
	// Create a game of TexasHoldem
	TexasHoldem game = TexasHoldem(Deck(), StraightIdentifier());
	game.addPlayer(individual1.getPlayer());
	game.addPlayer(individual2.getPlayer());
	game.playMultipleRounds(this->numGamesPerPair);
}

/*
Evaluates the score of each individual.
Each individual competes against 'numOpponents' random sampled individuals,
and the score is the number of times that the individual won.
*/
void GeneticAlgorithm::evaluate()
{
	// Make players to play against numOpponent players
	for (int i = 0; i < this->numIndividuals; i++)
	{
		if (this->currentIndividuals[i].getNumPlayedCompetitions() >= this->numOpponents)
			continue;

		for (int j = this->currentIndividuals[i].getNumPlayedCompetitions(); j < this->numOpponents; j++) {

			// Sample a random opponent
			int randIdx = rand() % this->numIndividuals;

			// Resample if is the same player as i or if the sampled player already played numOpponents times
			while (randIdx == i || (this->currentIndividuals[randIdx].getNumPlayedCompetitions() >= this->numOpponents))
			{
				randIdx = rand() % this->numIndividuals;
			}

			// Make individuals compete
			this->compete(this->currentIndividuals[i], this->currentIndividuals[randIdx]);
			
			// Update individuals' score after the competition
			this->currentIndividuals[i].updateScore();
			this->currentIndividuals[randIdx].updateScore();
			
			// Add 1 to number of played games
			this->currentIndividuals[i].addPlayedCompetition();
			this->currentIndividuals[randIdx].addPlayedCompetition();
		}
	}
}

void GeneticAlgorithm::selectBest(float ratio)
{
	std::random_device device;
	std::mt19937 generator(device()); // Seed the random number engine
	
	// Create discrete distribution given by the score of each individual
	std::vector<int> scores;
	for (int i = 0; i < this->currentIndividuals.size(); i++)
	{
		scores.push_back(this->currentIndividuals[i].getScore());
	}
	std::discrete_distribution<> distrib(scores.begin(), scores.end()); // Create the distribution
	
	// Sample from the distribution
	std::vector<Individual> newIndividuals;
	for (int i = 0; i < (int)(ratio * this->numIndividuals); i++)
	{
		int randIdx = distrib(generator);
		newIndividuals.push_back(this->currentIndividuals[randIdx]);
	}
	
	// Replace the current individuals
	this->currentIndividuals.clear();
	for (int i = 0; i < newIndividuals.size(); i++)
	{
		this->currentIndividuals.push_back(newIndividuals[i]);
	}
	this->numIndividuals = this->currentIndividuals.size();	
}

void GeneticAlgorithm::crossOver()
{
	
	for (int i = 0; i < this->numIndividuals - 1; i += 2)  // if 'numIndividuals' is odd, the last individual is skipped
	{
		std::vector<float> strategy1 = this->currentIndividuals[i].getPlayer().getStrategy();
		std::vector<float> strategy2 = this->currentIndividuals[i + 1].getPlayer().getStrategy();

		int crossOverIdx = rand() % (strategy1.size());
		this->currentIndividuals[i].crossOver(strategy2, crossOverIdx);
		this->currentIndividuals[i + 1].crossOver(strategy1, crossOverIdx);
	}
}

void GeneticAlgorithm::mutate(float probab)
{
	assert(probab <= 1.0f && probab >= 0.0f);

	int probabPercent = (int)probab * 100;
	for (int i = 0; i < this->numIndividuals; i++)  // if 'numIndividuals' is odd, the last individual is skipped
	{
		std::vector<float> strategy = this->currentIndividuals[i].getPlayer().getStrategy();

		std::vector<int> indexesToBeMutated = {};
		// Iterate over elements of the strategy array
		for (int j = 0; j < strategy.size(); j++)
		{
			// Mutate (change sign) with probability 'probab'
			int sample = rand() % 100;
			if (sample < probabPercent)
				indexesToBeMutated.push_back(j);
		}
		this->currentIndividuals[i].mutateStrategyElementByIndexVector(indexesToBeMutated);
	}
}

int GeneticAlgorithm::getNumIndividuals()
{
	return this->numIndividuals;
}

std::vector<float> GeneticAlgorithm::getIndividualStrategyByIndex(int idx)
{
	assert(idx >= 0);
	return this->currentIndividuals[idx].getPlayer().getStrategy();
}

Individual GeneticAlgorithm::getIndividualByIndex(int idx)
{
	return this->currentIndividuals.at(idx);
}
