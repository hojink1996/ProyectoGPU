#pragma once
#include "hand.h"

class Player
{
private:
	float stack;
	Hand& hand{ Hand() };
public:
	Player(float startingStack);
	void addCardToHand(std::pair<Suit, Value> card, int position);
};
