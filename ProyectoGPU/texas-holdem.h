#pragma once
#include <vector>
#include "deck.h"
#include "player.h"
#include "card.h"
#include "agent.h"

/*
The different types of combinations that a hand + shared card combination can return in
Texas Hold'em Poker.
*/
enum class HandValue
{
	HighCard,
	Pair,
	TwoPair,
	ThreeOfAKind,
	Straight,
	Flush,
	FullHouse,
	FourOfAKind,
	StraightFlush,
	RoyalFlush,
	Invalid
};

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
	std::array<Card, 5> sharedCards;
	bool isRoyalFlush(Hand& hand);
	bool isStraightFlush(Hand& hand);
	bool isFourOfAKind(Hand& hand);
	bool isFullHouse(Hand& hand);
	bool isFlush(Hand& hand);
	bool isStraight(Hand& hand);
	bool isThreeOfAKind(Hand& hand);
	bool isTwoPair(Hand& hand);
	bool isPair(Hand& hand);
public:
	TexasHoldem(int numPlayers, float startingStack, Agent& decisionAgent);
	TexasHoldem(int numPlayers, float startingStack, Deck& deck, Agent& decisionAgent);
	void resetDeck();
	HandValue evaluateHand(Hand& hand);
};

