#pragma once
#include <vector>
#include "deck.h"
#include "player.h"
#include "card.h"
#include "agent.h"

/*
The TexasHoldem Class is the main Class used to represent the game of Texas Hold'em
used in the simulations. The class handles the interaction between the different players
in the game, the deck and the bets they make. It also decides the winner after each round and
handles the resetting of the player's hand and deck.
*/
class TexasHoldem
{
private:
	Deck& currentDeck;
	int numPlayers;
	int dealerPosition;
	std::vector<Player> players;
	std::pair<Suit, Value> getNextCard();
public:
	TexasHoldem(int numPlayers, float startingStack, Agent& decisionAgent);
	TexasHoldem(int numPlayers, float startingStack, Deck& deck, Agent& decisionAgent);
	void resetDeck();
};

