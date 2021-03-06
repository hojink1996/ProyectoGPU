#pragma once
#include <vector>
#include <string>
#include "deck.h"
#include "player.h"
#include "card.h"
#include "agent.h"
#include "straight-identifier.h"
#include "state.h"

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
	bool print;
	Deck& currentDeck;
	StraightIdentifier& straightIdentifier;
	GameState currentGameState;
	int startingStack{};
	int minBet;
	int smallBlind;
	int bigBlind;
	int currentTotalBetAmount = 0;
	float* stateVector;
	int numPlayers{ 0 };
	int currentlyPlayingPlayers{ 0 };
	int dealerPosition{ 0 };
	int lastBet = 0;
	std::vector<bool> playerCurrentlyPlaying{};
	std::vector<Player*> players;
	std::array<Card, 5> sharedCards;

	/*
	Functions used to reset the state of the game once the playing round is over.

	Use 'endRound()' which calls 'resetDeck()', 'resetSharedCards()' and 'resetPlayerCards()'.
	*/
	void beginRound();
	void resetDeck();
	void resetSharedCards();
	void resetPlayers();
	void endRound();

	/*
	Functions used to advance the state of the game.
	
	The game of Texas Hold'em is separated in four parts. In the pre-flop stage, two cards are dealt to each of the players.
	Then, in the flop, three cards are dealt in the 'shared stack'. In the turn, one more card is dealt to the 'shared stack'.
	Finally, in the river, the last card for the 'shared stack' is dealt. The players can bet in each of the stages of the
	game.

	The 'dealCards()' function begins the pre-flop stage. Once the betting is done, the 'drawFlop()' function is used to
	pass to the flop. Then the 'drawTurn()' function is used to pass to the turn. Finally, the 'drawRiver()' function is used
	to pass to the last stage of the game.

	The 'bettingRound()' functions contains all of the logic for the betting between the players in the game.
	*/
	void dealCards();
	void drawFlop();
	void drawTurn();
	void drawRiver();
	void bettingRound();


	/*
	Functions used to evaluate whether a hand meets the requirements for a specific type of hand value.
	The naming convention is: 'is____', where '____' is the hand value we are evaluating if the hand meets.
	The function also gives a 'score' in the case it returns true. This score is meant to compare two hands
	that have the same hand value (e.g: two Full Houses), and determine which is more valuable. This score
	is returned in the 'handValue' parameter.

	The hand is represented by a vector that contains a combination of the player's cards and
	the shared cards. The maximum length for a vector is 7 (2 hands from the player's hand +
	5 shared hands). Behavior when given more than 7 cards cannot be ensured.

	These functions should not be used as standalone functions. They are used in the 'evaluateHand' function.
	This is because some of the functions assume that a certaing hand value is not possible. For instance,
	when verifying if a hand is a Straight Flush, we assume that the hand is not a Royal Flush.
	
	@parameter filteredBySuitAndOrdered:	A vector with the card values (Value type) for a given hand + shared card combination.
											The cards in the vector should have the same suit and a maximum length of 7 cards. Further,
											the cards should be ordered by their value.
											Behavior when these conditions are not met might be unexpected.
	@parameter orderedCards:				A vector with the card values (Value type) for a give hand + shared card combination.
											The cards in the vector should be ordered by their value, and the vector should have
											a maximum length of 7.
											Behavior when these conditions are not met might be unexpected.
	@parameter handValue:					An 'uint64_t' variable that will store the hand value in the case the function returns true.
											This variable should initially have a value of 0. Use this score to compare hands, only
											if they are the same type of hand (e.g: two Full Houses).
	@return:								The function return 'true' in case that a given hand meets the requirements to have
											a specific hand value. Otherwise, it returns 'false'. Further the parameter 'handValue'
											will only be modified if the function returns true.
	*/
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

	/*
	Function used to determine the winner for the current round of the game. This function can only be called
	once the 'drawRiver()' step of the game is called. This guarantees that each player has two cards in their
	hands and that there are five shared cards.

	@return:	A vector with the indices of the winning players.
	*/
	std::vector<int> determineWinner();

	/*
	Function used to assign the earnings of the round to the respective winning players.
	It takes the whole pot and divides it between the winning players (given by the 'winningPlayers' vector).
	
	@param winningPlayers:		A vector with the indices of the winning players.
	*/
	void assignEarningsToWinner(std::vector<int> winningPlayers);

	/*
	Additional auxiliary functions used.
	*/
	void updateFullHouse(int currentLength, bool& hasPair, bool& hasThreeOfAKind, Value& threeOfAKindValue,
		Value& pairValue, Value& currentCardValue);
	std::pair<Suit, Value> getNextCard();
	uint64_t computeCardValue(Value cardValue, int shiftedBitPositions);

	/*
	Functions to evaluate specific conditions of the game: Whether if all players have bet all their stack, 
	and whether if only one player is active in the game. 
	*/
	bool allPlayersAllIn();
	bool onlyOnePlayerLeft();

	/*
	Functions that construct the state vector of the game for a given player, assuming that the game only has two players.
	It contains:
	1. The last raised amount.
	2. The current amount in the pocket of the player.
	3. The current amount in the pocket of the opponent.
	4. The so-far bet amount.
	5. A vector of size 13 that contains the number of cards of each value that the player has (including the shared cards).
	6. A vector of size 4 that contains the number of cards of each suit that the player has (including the shared cards).

	@param state:			Reference to the state vector
	@param playerIndex:		Index of the player for which the state belongs
	*/
	void addLastBetDelta(State& state);
	void addMoneyLeftPlayer(State& state, int playerIndex);
	void addMoneyLeftOponent(State& state, int playerIndex);
	void addPlayerBetAmount(State& state, int playerIndex);
	void addCurrentHandCards(State& state, int playerIndex);
	void addCurrentHandSuits(State& state, int playerIndex);

	/*
	Print the cards in the terminal.
	*/
	void printPlayerHand(int playerIndex);
	void printSharedCards();

	/*
	Convert suit and value to string.
	*/
	std::string suitToString(Suit suit);
	std::string valueToString(Value value);
public:
	TexasHoldem(int numPlayers, Agent& decisionAgent, Deck& deck, StraightIdentifier& straightIdentifier, int startingStack=1000,
		int smallBlindValue=1, bool print=false);
	
	/*
	Constructor without defining the number of players, which is set to 0.
	*/
	TexasHoldem(Deck& deck, StraightIdentifier& straightIdentifier, float startingStack=1000, int smallBlindValue=1, 
		bool print=false);

	/*
	Function that evaluates the value of a hand.
	@param hand:		The hand of cards to be evaluated.
	@param handValue:	The initial hand value, set to 0.

	@return:	A HandValue object that indicates the categorty of the hand.
	*/
	HandValue evaluateHand(Hand& hand, uint64_t& handValue);

	/*
	Function that adds a player to the game.
	@param player:		Pointer to the player to be added.
	*/
	void addPlayer(Player* player);

	/*
	Function that simulate a round of the game.
	*/
	void playRound();

	/*
	Function to make the players play multiple times.
	*/
	void playMultipleRounds(int numberOfRounds);

	/*
	Set the value for the shared cards.
	*/
	void setSharedCards(std::array<Card, 5>& sharedCards);

	/*
	Function that returns the state vector of a player by its index.
	*/
	State getStateOfPlayer(int idx);
};


