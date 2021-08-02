#pragma once
#include <vector>
#include <chrono>
#include "individual.h"


class GeneticAlgorithm
{
private:
	int numIndividuals, numOpponents, numGamesPerPair;
	std::vector<Individual> newIndividuals;
	std::vector<Individual> currentIndividuals;
	std::vector<std::chrono::seconds> timePerGeneration;
	void compete(Individual& player1, Individual& player2);
	void resetIndividuals();
	float scoreOfTheBestAtPreviousEpoch = 0;
	int numThreads;
	bool training;
	bool parallelize;

public:
	GeneticAlgorithm(int iniNumIndividuals, int numOpponents, int numGamesPerPair=10, int numThreads=8, bool training=false,
		bool parallelize=true);

	/*
	Functions that execute each of the main blocks of the genetic algorithm: Evaluation, Selection, Crossing-Over and Mutation.
	*/
	float evaluate();
	void evaluatePairOfPlayers();
	void selectBest(float ratio);
	void crossOver();
	void mutate(float probab);

	/*
	Main function that computes the evolution for one generation. 
	It calls the evaluation, selection, crossOver and mutation functions.
	*/
	float trainOneEpoch(float selectBestRatio, float mutateProbab);

	/*
	Function that returns the execution time of each generation.
	*/
	std::vector<std::chrono::seconds> getTimePerGeneration();

	/*
	Getter functions for the number of individuals, the strategy of a individual,
	the vector of current individuals and a specific individual by index.
	*/
	int getNumIndividuals();
	std::vector<float> getIndividualStrategyByIndex(int idx);
	Individual getIndividualByIndex(int idx);
	std::vector<Individual> getCurrentIndividuals();
};