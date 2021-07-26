#include <iostream>
#include "context.h"
#include "genetic-algorithm.h"

using namespace std;

Context::Context()
{
}

void Context::run()
{
	int iniNumIndividuals = 4;
	int numOpponents = 2;
	int numGamesPerPair = 2;
	GeneticAlgorithm ga = GeneticAlgorithm(iniNumIndividuals, numOpponents, numGamesPerPair);

	float selectBestRatio = 0.8;
	float mutateProbab = 0.2;

	int numEpochs = 2;
	for (int i = 0; i < numEpochs; i++)
	{
		cout << "Epoch " << i << "/" << numEpochs << endl;
		ga.trainOneEpoch(selectBestRatio, mutateProbab);
	}

}
