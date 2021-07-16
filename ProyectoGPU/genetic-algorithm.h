#pragma once
#include <vector>
#include "player.h"


class GeneticAlgorithm
{
private:
	int numIndividuals, numOpponents;
	std::vector<Player> newIndividuals;
	int compete(Player player1, Player player2, int idx1, int idx2);
public:
	GeneticAlgorithm(int iniNumIndividuals, int numOpponents);
	void evaluate();
	void selectBest(float ratio);
	void crossOver();
	void mutate(float probab);

	int getNumIndividuals();
	std::vector<Player> currentIndividuals;
};