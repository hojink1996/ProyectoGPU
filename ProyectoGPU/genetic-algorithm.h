#pragma once
#include <vector>
#include "individual.h"


class GeneticAlgorithm
{
private:
	int numIndividuals, numOpponents, numGamesPerPair;
	std::vector<Individual> newIndividuals;
	void compete(Individual player1, Individual player2);

public:
	GeneticAlgorithm(int iniNumIndividuals, int numOpponents, int numGamesPerPair=10);
	void evaluate();
	void selectBest(float ratio);
	void crossOver();
	void mutate(float probab);

	int getNumIndividuals();
	std::vector<float> getIndividualStrategyByIndex(int idx);
	std::vector<Individual> currentIndividuals;
};