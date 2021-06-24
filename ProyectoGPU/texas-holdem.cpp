#include "texas-holdem.h"
#include "deck.h"
#include "player.h"
#include <vector>


TexasHoldem::TexasHoldem(int numPlayers, float startingStack, Agent& decisionAgent) : currentDeck(Deck())
{
	this->dealerPosition = 0;
	this->numPlayers = numPlayers;
	this->players = std::vector<Player>();
	for (int i = 0; i < numPlayers; ++i)
		this->players.push_back(Player(startingStack, decisionAgent));
}

TexasHoldem::TexasHoldem(int numPlayers, float startingStack, Deck& deck, Agent& decisionAgent) : currentDeck(deck)
{
	this->dealerPosition = 0;
	this->numPlayers = numPlayers;
	for (int i = 0; i < numPlayers; ++i)
		this->players.push_back(Player(startingStack, decisionAgent));
}

void TexasHoldem::resetDeck()
{
	this->currentDeck.reset();
}

Card TexasHoldem::getNextCard()
{
	return this->currentDeck.drawCard();
}

