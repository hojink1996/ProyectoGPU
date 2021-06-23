#pragma once
#include "hand.h"

class Player
{
private:
	float stack;
	Hand& hand{ Hand() };
	float lastBet{ 0.0f };
public:
	Player(float startingStack);
	void addCardToHand(std::pair<Suit, Value> card, int position);
};
