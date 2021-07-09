#pragma once
#include <vector>
#include "deck.h"
#include "player.h"
#include "card.h"
#include "agent.h"
#include "straight-identifier.h"

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
We need to save the current Game State for the game. This makes it possible to determine the
next step for the game.
*/
enum class GameState
{
	PreFlop,
	Flop,
	Turn,
	River,
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
	StraightIdentifier& straightIdentifier;
	GameState currentGameState;
	int numPlayers;
	int dealerPosition;
	std::vector<Player> players;
	std::pair<Suit, Value> getNextCard();
	std::array<Card, 5> sharedCards;
	uint64_t computeCardValue(Value cardValue, int shiftedBitPositions);
	void resetDeck();
	void resetSharedCards();
	void resetPlayerCards();
	void endRound();
	void dealCards();
	void drawFlop();
	void drawTurn();
	void drawRiver();
	void updateFullHouse(int currentLength, bool& hasPair, bool& hasThreeOfAKind, Value& threeOfAKindValue,
		Value& pairValue, Value& currentCardValue);
	bool isRoyalFlush(const std::vector<Value>& filteredBySuitAndOrdered, uint64_t& handValue);
	bool isStraightFlush(const std::vector<Value>& filteredBySuitAndOrdered, uint64_t& handValue);
	bool isFourOfAKind(const std::vector<Value>& orderedCards, uint64_t& handValue);
	bool isFullHouse(const std::vector<Value>& orderedCards, uint64_t& handValue);
	bool isFlush(const std::vector<Value>& filteredBySuitAndOrdered, uint64_t& handValue);
	bool isStraight(const std::vector<Value>& orderedCards, uint64_t& handValue);
	bool isThreeOfAKind(const std::vector<Value>& orderedCards, uint64_t& handValue);
	bool isTwoPair(const std::vector<Value>& orderedCards, uint64_t& handValue);
	bool isPair(const std::vector<Value>& orderedCards, uint64_t& handValue);
	bool isHighCard(const std::vector<Value>& orderedCards, uint64_t& handValue);
	int determineWinner();
public:
	TexasHoldem(int numPlayers, float startingStack, Agent& decisionAgent, Deck& deck, StraightIdentifier& straightIdentifier);
	HandValue evaluateHand(Hand& hand, uint64_t& handValue);
	void setSharedCards(std::array<Card, 5>& sharedCards);
};

