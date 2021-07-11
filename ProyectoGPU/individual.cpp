#include "individual.h"

Individual::Individual(Player& player) : player(player)
{
	this->numPlayedGames = 0;
	this->numWins = 0;
}

void Individual::registerWin()
{
	this->numWins++;
}

void Individual::addPlayedGame()
{
	this->numPlayedGames++;
}

int Individual::getNumPlayedGames()
{
	return this->numPlayedGames;
}

int Individual::getNumWins()
{
	return this->numWins;
}
