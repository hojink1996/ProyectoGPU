#pragma once
#include "hand.h"

class Player
{
private:
	float stack;
	Hand hand;
public:
	Player(float startingStack);
};
