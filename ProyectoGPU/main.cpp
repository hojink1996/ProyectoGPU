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

using namespace std;

void main()
{
	/*
	LinearAgent agent;
	agent.initialize_theta();
	agent.print_theta();

	float state[10] = { 0.0f, 1.0f, 10.0f, -2.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	float maxBet = 10.0f;
	float minBet = 1.0f;
	Decision decision = agent.makeDecision(state, maxBet, minBet);

	cout << "FOLD: " << (decision.play == Play::Fold) << endl;
	cout << "CALL: " << (decision.play == Play::Call) << endl;
	cout << "RAISE: " << (decision.play == Play::Raise) << endl;
	cout << "Invalid: " << (decision.play == Play::Invalid) << endl;

	cout << "Amount: " << decision.betAmount << endl;

	*/
	
	LinearAgent agent = LinearAgent(20);

	int iniNumIndividuals = 3;
	int numOpponents = 2;
	GeneticAlgorithm ga = GeneticAlgorithm(iniNumIndividuals, numOpponents);
	
	
	ga.evaluate();
	
	int totalWin = 0;

	// Assert all individuals competed 'numOpponents' times
	for (int i = 0; i < ga.getNumIndividuals(); i++)
	{
		cout << ga.getIndividualByIndex(i).getNumPlayedCompetitions() << endl;
	}

	cout << "Total win: " << totalWin << endl;

	ga.selectBest(1.0);
	

	ga.crossOver();
	ga.mutate(0.5);

	return;
}