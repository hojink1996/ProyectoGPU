#include <iostream>
#include <random>
#include "genetic-algorithm.h"
#include "agent.h"
#include "texas-holdem.h"

GeneticAlgorithm::GeneticAlgorithm(int iniNumIndividuals, int numOpponents)
{
	this->numIndividuals = iniNumIndividuals;
	this->numOpponents = numOpponents;
	for (int i = 0; i < this->numIndividuals; i++) {
		LinearAgent agent = LinearAgent();
		agent.initialize_theta();
		int startingStack = 1000;
		Player player = Player(startingStack, agent);
		this->currentIndividuals.push_back(Individual(player));
	}
}


int GeneticAlgorithm::compete(Individual individual1, Individual individual2, int idx1, int idx2)
{
	/*
	* // Create a game of TexasHoldem
	LinearAgent agent = LinearAgent();
	Deck deck = Deck();
	StraightIdentifier straightIdentifier = StraightIdentifier();
	TexasHoldem texasHoldemGame = TexasHoldem(1, 100.0f, agent, deck, straightIdentifier);

	*/
	int randIdx = rand() % 2;
	if (randIdx)
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
	// std::random_device device;
	// std::mt19937 engine(device()); // Seed the random number engine
	// std::discrete_distribution<> dist({ 150, 40, 15, 3 }); // Create the distribution

	// // Now generate values with:
	// std::cout << (dist(engine) + 1) << std::endl;

	// Uniform sampling
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> distrib(0, this->numIndividuals - 1);

	std::vector<Individual> newIndividuals;
	for (int i = 0; i < (int)(ratio * this->numIndividuals); i++)
	{
		int randIdx = distrib(gen);
		newIndividuals.push_back(this->currentIndividuals[randIdx]);
	}
	
	this->currentIndividuals.clear();

	for (int i = 0; i < newIndividuals.size(); i++)
	{
		this->currentIndividuals.push_back(newIndividuals[i]);
	}
	
	
	// Empty vector of newIndividuals
	newIndividuals.clear();
	

}

void GeneticAlgorithm::crossOver()
{

}

void GeneticAlgorithm::mutate()
{
}

int GeneticAlgorithm::getNumIndividuals()
{
	return this->numIndividuals;
}
