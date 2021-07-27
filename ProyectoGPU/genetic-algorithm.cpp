#include <iostream>
#include <random>
#include <cassert>
#include "genetic-algorithm.h"
#include "agent.h"
#include "texas-holdem.h"
#include "individual.h"
#include "operations.cuh"

GeneticAlgorithm::GeneticAlgorithm(int iniNumIndividuals, int numOpponents, int numGamesPerPair)
{
	assert(iniNumIndividuals > 1);
	assert(numOpponents > 0);

	this->numIndividuals = iniNumIndividuals;
	this->numOpponents = numOpponents;
	this->numGamesPerPair = numGamesPerPair;
	//this->currentIndividuals = {};

	int startingStack = 5;	
	for (int i = 0; i < this->numIndividuals; i++) 
	{
		LinearAgent* agent = new LinearAgent(2);
		Player* player = new Player(startingStack, *agent);
		Individual individual = Individual(player);
		this->currentIndividuals.push_back(individual);
	}
}


void GeneticAlgorithm::compete(Individual& individual, Individual& individual2)
{
	// Create a game of TexasHoldem
	TexasHoldem game = TexasHoldem(Deck(), StraightIdentifier(), 2.0f);
	game.addPlayer(individual.getPlayer());
	game.addPlayer(individual2.getPlayer());
	game.playMultipleRounds(this->numGamesPerPair);
}

void GeneticAlgorithm::resetIndividuals()
{
	for (int i = 0; i < this->numIndividuals; i++)
	{
		this->currentIndividuals[i].reset();
	}
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
		for (int j = 0; j < this->numOpponents; j++) {

			// Sample a random opponent (of course different than i)
			int randIdx = rand() % this->numIndividuals;
			while (randIdx == i)
			{
				randIdx = rand() % this->numIndividuals;
			}

			// Let individuals compete
			this->compete(this->currentIndividuals[i], this->currentIndividuals[randIdx]);
			
			// Update individuals' score after the competition
			this->currentIndividuals[i].updateScore();
			this->currentIndividuals[randIdx].updateScore();
			
			// Add  to number of played games
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
	std::vector<float> scores;
	for (int i = 0; i < this->currentIndividuals.size(); i++)
	{
		// Convert score with Softplus so that all scores are positive
		float rawScore = this->currentIndividuals[i].getScore();
		//float convertedScore = std::log(1.0f + std::exp(rawScore));
		scores.push_back(rawScore);
	}

	std::vector<float> output(this->numIndividuals, 1);
	CudaFunctions::softplus(&scores[0], this->currentIndividuals.size(), &output[0]);


	std::discrete_distribution<> distrib(output.begin(), output.end()); // Create the distribution
	

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
		std::vector<float> strategy = (*this->currentIndividuals[i].getPlayer()).getStrategy();
		std::vector<float> strategy2 = (*this->currentIndividuals[i + 1].getPlayer()).getStrategy();

		int crossOverIdx = rand() % (strategy.size());
		this->currentIndividuals[i].crossOver(strategy2, crossOverIdx);
		this->currentIndividuals[i + 1].crossOver(strategy, crossOverIdx);
	}
}

void GeneticAlgorithm::mutate(float probab)
{
	assert(probab <= 1.0f && probab >= 0.0f);

	int probabPercent = (int)probab * 00;
	for (int i = 0; i < this->numIndividuals; i++)  // if 'numIndividuals' is odd, the last individual is skipped
	{
		std::vector<float> strategy = (*this->currentIndividuals[i].getPlayer()).getStrategy();

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
	return (*this->currentIndividuals[idx].getPlayer()).getStrategy();
}

Individual GeneticAlgorithm::getIndividualByIndex(int idx)
{
	return this->currentIndividuals.at(idx);
}

void GeneticAlgorithm::trainOneEpoch(float selectBestRatio, float mutateProbab)
{
	std::cout << "Evaluating..." << std::endl;
	this->evaluate();

	std::cout << "Selecting best individuals..." << std::endl;
	this->selectBest(selectBestRatio);

	std::cout << "Crossing over..." << std::endl;
	this->crossOver();

	std::cout << "Mutating..." << std::endl;
	this->mutate(mutateProbab);

	this->resetIndividuals();
}


