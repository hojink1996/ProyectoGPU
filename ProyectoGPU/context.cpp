#include <iostream>
#include "context.h"
#include "genetic-algorithm.h"

using namespace std;

Context::Context(bool parallelize=true)
{
	this->parallelize = parallelize;
}

void Context::run()
{
	int iniNumIndividuals = 1000;
	int numOpponents = 5;
	int numGamesPerPair = 5;
	GeneticAlgorithm ga = GeneticAlgorithm(iniNumIndividuals, numOpponents, numGamesPerPair);

	float selectBestRatio = 1.0;
	float mutateProbab = 0.2;

	int numEpochs = 10;
	for (int i = 0; i < numEpochs; i++)
	{
		cout << "Epoch " << i << "/" << numEpochs << endl;
		ga.trainOneEpoch(selectBestRatio, mutateProbab);
	}

}
