#pragma once
#include "player.h"
class Individual
{
private:
	Player& player;
	int numPlayedCompetitions;
	float score;
public:
	Individual(Player& player);
	void addPlayedCompetition();
	int getNumPlayedCompetitions();
	Player& getPlayer();
	void updateScore();
	float getScore();
};