#pragma once
#include <vector>
#include "deck.h"
#include "player.h"
#include "card.h"

class TexasHoldem
{
private:
	Deck currentDeck;
	int numPlayers;
	int dealerPosition;
	std::vector<Player> players;
public:
	TexasHoldem(int numPlayers, float startingStack);
	TexasHoldem(int numPlayers, float startingStack, Deck deck);
	void shuffleDeck();
	std::pair<Suit, Value> getNextCard();
};

