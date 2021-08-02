#include <array>
#include <iostream>
#include <fstream>
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

/*
Save the loss por epoch in a text file.
@param epoch:	The current epoch
@param text:	A string that contains the loss value
*/
void saveLoss(int epoch, const std::string& text)
{
	ofstream log_file("logs/loss.txt", ios_base::out | ios_base::app);
	log_file << "Epoch " << epoch << " - " << text << endl;
}

/*
Write in the log file.
*/
void writeLog(const std::string& text)
{
	ofstream log_file("logs/loss.txt", ios_base::out | ios_base::app);
	log_file << text << endl;
}



void main()
{
	int iniNumIndividuals = 1000;
	int numOpponents = 5;
	int numGamesPerPair = 20;
	int saveEvery = 10;
	int numThreads = 32;
	bool training = true;
	bool parallelize;
	if (numThreads == 1)
		parallelize = false;
	else
		parallelize = true;
	GeneticAlgorithm ga = GeneticAlgorithm(iniNumIndividuals, numOpponents, numGamesPerPair, numThreads, training, parallelize);

	cout << "Num individuals: " << iniNumIndividuals << endl;
	cout << "Num opponents: " << numOpponents << endl;
	cout << "Num games per pair: " << numGamesPerPair << endl;
	cout << "Num threads: " << numThreads << endl;
	cout << "Parallelize: " << parallelize << endl;

	writeLog("\n------------------------------------\n");
	writeLog("Num individuals: " + to_string(iniNumIndividuals));
	writeLog("Num opponents: " + to_string(numOpponents));
	writeLog("Num games per pair: " + to_string(numGamesPerPair));
	writeLog("Num threads: " + to_string(numThreads));
	writeLog("Parallelize: " + to_string(parallelize) + "\n");


	float selectBestRatio = 1.0;
	float mutateProbab = 0.1;

	int numEpochs = 1000;
	for (int i = 0; i < numEpochs; i++)
	{
		cout << "Epoch " << i << "/" << numEpochs << endl;

		float loss = ga.trainOneEpoch(selectBestRatio, mutateProbab);
		std::cout << "Difference of score of the best individual: " << loss << std::endl;
		saveLoss(i, std::to_string(loss));

		if ((i % saveEvery) == 0) {
			std::cout << "Saving at epoch: " << i << std::endl;
			int index = 0;
			for (auto individual : ga.getCurrentIndividuals())
			{
				ofstream data_file;
				data_file.open("individual_" + std::to_string(index) + "epoch_" + std::to_string(i) + ".bin", ios::out | ios::binary);
				std::vector<float> parameter = individual.getPlayer()->getAgent().getTheta();
				size_t size = parameter.size();
				for(int i = 0; i < size; ++i)
					data_file.write(reinterpret_cast<char*>(&parameter[i]), sizeof(float));
				data_file.close();
				++index;
			}
		}
	}
	int index = 0;
	for (auto individual : ga.getCurrentIndividuals())
	{
		ofstream data_file;
		data_file.open("individual_" + std::to_string(index) + ".bin", ios::out | ios::binary);
		std::vector<float> parameter = individual.getPlayer()->getAgent().getTheta();
		size_t size = parameter.size();
		for (int i = 0; i < size; ++i)
			data_file.write(reinterpret_cast<char*>(&parameter[i]), sizeof(float));
		data_file.close();
		++index;
	}
	std::vector<std::chrono::seconds> times = ga.getTimePerGeneration();
	int epoch = 0;
	for (auto time : times)
	{
		std::cout << "Epoch " << epoch << " took: " << time.count() << " s" << std::endl;
		++epoch;
	}

	return;
	
}
/*
void main()
{
	std::vector<float> linearIndividual;
	int individual = 2;
	int epoch = 100;
	int NUMBER_OF_ITEMS = 21;
	ifstream data_file;      // NOW it's ifstream
	data_file.open("individual_" + std::to_string(individual) + "epoch_" + std::to_string(epoch) + ".bin", ios::in | ios::binary);
	linearIndividual.resize(NUMBER_OF_ITEMS);
	data_file.read(reinterpret_cast<char*>(&linearIndividual[0]), NUMBER_OF_ITEMS * sizeof(float));
	data_file.close();

	LinearAgent agent = LinearAgent(linearIndividual);
	Player player = Player(agent);
	Deck deck = Deck();
	StraightIdentifier identifier = StraightIdentifier();
	TexasHoldem game = TexasHoldem(deck, identifier, 100.0f, 1, true);
	game.addPlayer(&player);

	InputAgent playerAgent = InputAgent();
	Player inputPlayer = Player(playerAgent);
	game.addPlayer(&inputPlayer);
	game.playMultipleRounds(5);
}
*/
