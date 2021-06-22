#include "texas-holdem.h"
#include "deck.h"
#include "player.h"
#include <vector>


TexasHoldem::TexasHoldem(int numPlayers, float startingStack)
{
	this->dealerPosition = 0;
	this->numPlayers = numPlayers;
	this->currentDeck = Deck();
	this->players = std::vector<Player>();
	for (int i = 0; i < numPlayers; ++i)
		this->players.push_back(Player(startingStack));
}

TexasHoldem::TexasHoldem(int numPlayers, float startingStack, Deck deck)
{
	this->dealerPosition = 0;
	this->numPlayers = numPlayers;
	this->currentDeck = deck;
	for (int i = 0; i < numPlayers; ++i)
		this->players.push_back(Player(startingStack));
}

void TexasHoldem::shuffleDeck()
{
	this->currentDeck.shuffle();
}
