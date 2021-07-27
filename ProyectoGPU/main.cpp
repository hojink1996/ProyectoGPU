#include <array>
#include <iostream>
#include "CppUnitTest.h"
#include "../ProyectoGPU/deck.h"
#include "../ProyectoGPU/card.h"
#include "../ProyectoGPU/hand.h"
#include "../ProyectoGPU/player.h"
#include "../ProyectoGPU/straight-identifier.h"
#include "../ProyectoGPU/texas-holdem.h"
#include "../ProyectoGPU/agent.h"
#include "../ProyectoGPU/genetic-algorithm.h"
#include "../ProyectoGPU/context.h"
#include "operations.cuh"

using namespace std;

void main()
{
	int iniNumIndividuals = 100;
	int numOpponents = 5;
	int numGamesPerPair = 5;
	GeneticAlgorithm ga = GeneticAlgorithm(iniNumIndividuals, numOpponents, numGamesPerPair);

	float selectBestRatio = 1.0;
	float mutateProbab = 0.1;

	int numEpochs = 10;
	for (int i = 0; i < numEpochs; i++)
	{
		cout << "Epoch " << i << "/" << numEpochs << endl;
		ga.trainOneEpoch(selectBestRatio, mutateProbab);
	}
	
	return;
}