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
	this->timePerGeneration = {};
	
	for (int i = 0; i < this->numIndividuals; i++) 
	{
		LinearAgent* agent = new LinearAgent(21);
		Player* player = new Player(*agent);
		Individual individual = Individual(player);
		this->currentIndividuals.push_back(individual);
	}
}


void GeneticAlgorithm::compete(Individual& individual, Individual& individual2)
{
	// Create a game of TexasHoldem
	TexasHoldem game = TexasHoldem(Deck(), StraightIdentifier());
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
		float rawScore = this->currentIndividuals[i].getScore() / (this->currentIndividuals[i].getNumPlayedCompetitions());
		scores.push_back(rawScore);
	}

	std::vector<float> transformedScores(this->numIndividuals, 0);
	CudaFunctions::softplus(&scores[0], this->currentIndividuals.size(), &transformedScores[0]);

	std::discrete_distribution<> distrib(transformedScores.begin(), transformedScores.end()); // Create the distribution

	// Sample from the distribution
	std::vector<Individual> newIndividuals;
	float meanScore = 0;
	int nextGenerationSize = (int)(ratio * this->numIndividuals);
	
	std::vector<std::vector<float>> sampledStrategies;
	for (int i = 0; i < nextGenerationSize; i++)
	{
		int randIdx = distrib(generator);
		Individual* newIndividual = this->currentIndividuals[randIdx].clone();
		newIndividuals.push_back(*newIndividual);
		meanScore += scores[randIdx] / nextGenerationSize;
	}
	std::cout << "Average score: " << meanScore << std::endl;

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

		std::random_device device_random;
		std::default_random_engine generator(device_random());
		std::normal_distribution<> distribution(0, 1);

		std::vector<float> noise(strategy.size(), 0);
		std::vector<int> mask(strategy.size(), 0); 

		// Iterate over elements of the strategy array
		for (int j = 0; j < strategy.size(); j++)
		{
			noise.at(j) = distribution(generator);
			int sample = rand() % 100;
			if (sample < 80)
				mask.at(j) = 1;
		}

		this->currentIndividuals[i].mutateStrategyElementByIndexVector(noise, mask);
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
	auto preRun = std::chrono::high_resolution_clock::now();
	std::cout << "Evaluating..." << std::endl;
	this->evaluate();

	std::cout << "Selecting best individuals..." << std::endl;
	this->selectBest(selectBestRatio);

	std::cout << "Crossing over..." << std::endl;
	this->crossOver();

	std::cout << "Mutating..." << std::endl;
	this->mutate(mutateProbab);
	auto afterRun = std::chrono::high_resolution_clock::now();
	this->timePerGeneration.push_back(std::chrono::duration_cast<std::chrono::seconds>(afterRun - preRun));

	this->resetIndividuals();
}


std::vector<std::chrono::seconds> GeneticAlgorithm::getTimePerGeneration()
{
	return this->timePerGeneration;
}

