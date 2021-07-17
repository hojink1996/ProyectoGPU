#include <iostream>
#include <random>
#include "genetic-algorithm.h"
#include "agent.h"
#include "texas-holdem.h"

GeneticAlgorithm::GeneticAlgorithm(int iniNumIndividuals, int numOpponents)
{
	this->numIndividuals = iniNumIndividuals;
	this->numOpponents = numOpponents;

	int startingStack = 1000;	
	for (int i = 0; i < this->numIndividuals; i++) 
	{
		LinearAgent* agent = new LinearAgent(20);
		Player player = Player(startingStack, agent);
		this->currentIndividuals.push_back(player);
	}
}


int GeneticAlgorithm::compete(Player player1, Player player2, int idx1, int idx2)
{
	// Create a game of TexasHoldem
	TexasHoldem game = TexasHoldem(Deck(), StraightIdentifier());
	game.addPlayer(player1);
	game.addPlayer(player2);
	game.play();
	int winnerIdx = game.determineWinner(); 
	if (winnerIdx == 0)
		return idx1;
	return idx2;

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
		if (this->currentIndividuals[i].getNumPlayedGames() >= this->numOpponents)
			continue;

		for (int j = this->currentIndividuals[i].getNumPlayedGames(); j < this->numOpponents; j++) {

			// Sample a random opponent
			int randIdx = rand() % this->numIndividuals;

			// Resample if is the same player as i or if the sampled player already played numOpponents times
			while (randIdx == i || (this->currentIndividuals[randIdx].getNumPlayedGames() >= this->numOpponents))
			{
				randIdx = rand() % this->numIndividuals;
			}

			// Make individuals compete
			int winnerIdx = this->compete(this->currentIndividuals[i], this->currentIndividuals[randIdx], i, randIdx);
			
			// Add score to winner
			this->currentIndividuals[winnerIdx].registerWin();
			
			// Add 1 to number of played games
			this->currentIndividuals[i].addPlayedGame();
			this->currentIndividuals[randIdx].addPlayedGame();
		}
	}
}

void GeneticAlgorithm::selectBest(float ratio)
{
	// TODO: sample from discrete distribution
	std::random_device device;
	std::mt19937 generator(device()); // Seed the random number engine
	
	// Create discrete distribution given by the score of each individual
	std::vector<int> scores;
	for (int i = 0; i < this->currentIndividuals.size(); i++)
	{
		scores.push_back(this->currentIndividuals[i].getNumWins());
	}
	std::discrete_distribution<> distrib(scores.begin(), scores.end()); // Create the distribution
	
	// Sample from the distribution
	std::vector<Player> newIndividuals;
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
		float* strategy1 = this->currentIndividuals[i].getStrategy();
		float* strategy2 = this->currentIndividuals[i + 1].getStrategy();

		int strategySize = this->currentIndividuals[i].getStrategySize();
		int crossOverIdx = rand() % strategySize;


		for (int j = crossOverIdx; j < strategySize; j++)
		{
			float aux = strategy1[j];
			strategy1[j] = strategy2[j];
			strategy2[j] = aux;
		}
	}
}

void GeneticAlgorithm::mutate(float probab)
{
	int probabPercent = (int)probab * 100;
	for (int i = 0; i < this->numIndividuals; i++)  // if 'numIndividuals' is odd, the last individual is skipped
	{
		float* strategy = this->currentIndividuals[i].getStrategy();
		int strategySize = this->currentIndividuals[i].getStrategySize();

		// Iterate over elements of the strategy array
		for (int j = 0; j < strategySize; j++)
		{
			// Mutate (change sign) with probability 'probab'
			int sample = rand() % 100;
			if (sample < probabPercent)
				strategy[j] = -strategy[j];
		}
	}
}

int GeneticAlgorithm::getNumIndividuals()
{
	return this->numIndividuals;
}

float* GeneticAlgorithm::getIndividualStrategyByIndex(int idx)
{
	return this->currentIndividuals[idx].getStrategy();
}
