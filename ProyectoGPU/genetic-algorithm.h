#pragma once
#include <vector>
#include "individual.h"


class GeneticAlgorithm
{
private:
	int numIndividuals, numOpponents;
	std::vector<Individual> newIndividuals;
	int compete(Individual individual1, Individual individual2, int idx1, int idx2);
public:
	GeneticAlgorithm(int iniNumIndividuals, int numOpponents);
	void evaluate();
	void selectBest(float ratio);
	void crossOver();
	void mutate();
	std::vector<Individual> currentIndividuals;

	int getNumIndividuals();
};