#pragma once
#include <vector>
#include <chrono>
#include "individual.h"


class GeneticAlgorithm
{
private:
	int numIndividuals, numOpponents, numGamesPerPair;
	int numThreads;
	std::vector<Individual> newIndividuals;
	std::vector<Individual> currentIndividuals;
	std::vector<std::chrono::seconds> timePerGeneration;
	void compete(Individual& player1, Individual& player2);
	void resetIndividuals();
	void evaluatePairOfPlayers();
	float scoreOfTheBestAtPreviousEpoch = 0;

public:
	GeneticAlgorithm(int iniNumIndividuals, int numOpponents, int numGamesPerPair = 10, int numThreads = 20);
	void evaluate();
	void selectBest(float ratio);
	void crossOver();
	void mutate(float probab);
	std::vector<std::chrono::seconds> getTimePerGeneration();

	int getNumIndividuals();
	std::vector<float> getIndividualStrategyByIndex(int idx);
	Individual getIndividualByIndex(int idx);

	void trainOneEpoch(float selectBestRatio, float mutateProbab);
};