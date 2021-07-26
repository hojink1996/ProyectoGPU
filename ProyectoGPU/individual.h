#pragma once
#include "player.h"
class Individual
{
private:
	Player& player;
	Player* playerPointer;
	int numPlayedCompetitions;
	float score;
public:
	Individual(Player* player);
	void addPlayedCompetition();
	int getNumPlayedCompetitions();
	Player* getPlayer();
	void updateScore();
	float getScore();
	void crossOver(std::vector<float> strategy, int idx);
	void mutateStrategyElementByIndexVector(std::vector<int> indexesToBeMutated);
};