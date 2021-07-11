#pragma once
#include <vector>
#include "player.h"

class Individual
{
private:
	Player& player;
	int numPlayedGames;
	int numWins;
public:
	Individual(Player& player);
	void registerWin();
	void addPlayedGame();
	int  getNumPlayedGames();
	int getNumWins();
}; 