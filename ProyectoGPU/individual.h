#pragma once
#include "player.h"
class Individual
{
private:
	Player& player;
	Player* playerPointer;
	int numPlayedCompetitions;
	bool currentlyPlaying;
	float score;
public:
	Individual(Player* player);
	void addPlayedCompetition();
	int getNumPlayedCompetitions();
	Player* getPlayer();
	void updateScore();
	float getScore();
	void crossOver(std::vector<float> strategy, int idx);
	void mutateStrategyElementByIndexVector(std::vector<float> noise, std::vector<int> mask);
	void beginPlaying();
	void endPlaying();
	bool isCurrentlyPlaying();

	void reset();
	Individual* clone();
};