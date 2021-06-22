#pragma once
#include <array>
class Deck
{
private:
	std::array<int, 52> cards;
public:
	Deck(std::array<int, 52> cards);
	Deck();
	void shuffle();
	void reset();
};
