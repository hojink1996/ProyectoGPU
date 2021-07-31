#include "texas-holdem.h"
#include "deck.h"
#include "player.h"
#include <algorithm>
#include <vector>
#include <iostream>
#include <cstdint>
#include <cassert>

TexasHoldem::TexasHoldem(int numPlayers, Agent& decisionAgent, Deck& deck,
	StraightIdentifier& straightIdentifier, int startingStack, int smallBlindValue) : currentDeck(deck), straightIdentifier(straightIdentifier)
{
	this->startingStack = startingStack;
	this->dealerPosition = 0;
	this->currentGameState = GameState::Invalid;
	this->numPlayers = numPlayers;
	this->sharedCards = { invalidCard, invalidCard, invalidCard, invalidCard, invalidCard };
	this->playerCurrentlyPlaying = {};
	this->lastBet = 0;
	for (int i = 0; i < numPlayers; ++i)
	{
		Player* player = new Player(decisionAgent);
		(*player).assignStack(this->startingStack);
		this->players.push_back(player);
		this->playerCurrentlyPlaying.push_back(true);
	}
	this->smallBlind = smallBlindValue;
	this->bigBlind = 2 * smallBlindValue;
	this->minBet = smallBlindValue;
}

/*
Constructor without specifying number of players.
*/
TexasHoldem::TexasHoldem(Deck& deck, StraightIdentifier& straightIdentifier, float startingStack,
	int smallBlindValue) : currentDeck(deck), straightIdentifier(straightIdentifier)
{
	this->startingStack = startingStack;
	this->numPlayers = 0;
	this->dealerPosition = 0;
	this->currentGameState = GameState::Invalid;
	this->sharedCards = { invalidCard, invalidCard, invalidCard, invalidCard, invalidCard };
	this->playerCurrentlyPlaying = {};	
	this->lastBet = 0;
	this->smallBlind = smallBlindValue;
	this->bigBlind = 2 * smallBlindValue;
	this->minBet = smallBlindValue;
}

void TexasHoldem::addPlayer(Player* player)
{
	this->numPlayers++;
	(*player).assignStack(this->startingStack);
	this->players.push_back(player);
	this->playerCurrentlyPlaying.push_back(true);
}

void TexasHoldem::resetDeck()
{
	this->currentDeck.reset();
}

Card TexasHoldem::getNextCard()
{
	return this->currentDeck.drawCard();
}

void TexasHoldem::dealCards()
{
	this->currentGameState = GameState::PreFlop;
	// We give 2 cards to each player
	for (int cardNumber = 0; cardNumber < 2; ++cardNumber)
	{
		for (auto player : this->players)
		{
			(*player).addCardToHand(this->getNextCard(), cardNumber);
		}
	}
}

void TexasHoldem::drawFlop()
{
	this->currentGameState = GameState::Flop;
	if (this->onlyOnePlayerLeft())
		return;
	for (int cardNumber = 0; cardNumber < 3; ++cardNumber)
		sharedCards[cardNumber] = this->getNextCard();
}

void TexasHoldem::drawTurn()
{
	this->currentGameState = GameState::Turn;
	if (this->onlyOnePlayerLeft())
		return;
	sharedCards[3] = this->getNextCard();
}

void TexasHoldem::drawRiver()
{
	this->currentGameState = GameState::River;
	if (this->onlyOnePlayerLeft())
		return;
	sharedCards[4] = this->getNextCard();
}

void TexasHoldem::resetSharedCards()
{
	this->sharedCards = { invalidCard, invalidCard, invalidCard, invalidCard, invalidCard };
}

void TexasHoldem::resetPlayers()
{
	for (int i=0; i<this->numPlayers; i++)
	{
		(*this->players[i]).resetHand();
		(*this->players[i]).setPlayerStack(this->startingStack);
	}
}

void TexasHoldem::beginRound()
{
	this->currentlyPlayingPlayers = this->numPlayers;
	this->currentTotalBetAmount = 0;
	for (int index = 0; index < this->playerCurrentlyPlaying.size(); ++index)
	{
		this->playerCurrentlyPlaying.at(index) = true;
	}
}

void TexasHoldem::endRound()
{
	/*
	// Players that ended up with empty stack are not playing anymore
	for (int index = 0; index < this->playerCurrentlyPlaying.size(); ++index)
	{
		this->playerCurrentlyPlaying.at(index) = false;
	}
	*/
	this->resetDeck();
	this->resetPlayers();
	this->resetSharedCards();
	this->dealerPosition = (this->dealerPosition + 1) % (this->numPlayers);
	GameState::Invalid;
}

uint64_t TexasHoldem::computeCardValue(Value cardValue, int shiftedPositions)
{
	int setBitPosition = (static_cast<int>(cardValue) + 12) % 13;
	uint64_t initialValue = 1;
	return initialValue << (setBitPosition + 13 * shiftedPositions);
}

bool TexasHoldem::isRoyalFlush(const std::vector<Value>& filteredBySuitAndOrdered, uint64_t& handValue)
{
	Value cardValue = Value::Invalid;
	bool handIsRoyalFlush = this->straightIdentifier.hasRoyalStraight(filteredBySuitAndOrdered, cardValue);
	if (handIsRoyalFlush)
		handValue += this->computeCardValue(cardValue, 0);
	return handIsRoyalFlush;
}

bool TexasHoldem::isStraightFlush(const std::vector<Value>& filteredBySuitAndOrdered, uint64_t& handValue)
{
	Value cardValue = Value::Invalid;
	bool handIsStraightFlush = this->straightIdentifier.hasStraightFlush(filteredBySuitAndOrdered, cardValue);
	if (handIsStraightFlush)
		handValue += this->computeCardValue(cardValue, 0);
	return handIsStraightFlush;
}

bool TexasHoldem::isStraight(const std::vector<Value>& orderedCards, uint64_t& handValue)
{
	Value cardValue = Value::Invalid;
	bool handIsStraight = this->straightIdentifier.hasStraight(orderedCards, cardValue);
	if (handIsStraight)
		handValue += this->computeCardValue(cardValue, 0);
	return handIsStraight;
}

bool TexasHoldem::isFourOfAKind(const std::vector<Value>& orderedCards, uint64_t& handValue)
{		
	handValue = 0;
	int currentLength = 0;
	Value currentCardValue = orderedCards[0];
	Value highestCard = Value::Invalid;
	Value fourOfAKind = Value::Invalid;
	for (int currentCard = 0; currentCard < orderedCards.size(); ++currentCard)
	{
		if (currentCardValue == orderedCards[currentCard])
			++currentLength;
		else
		{
			if (currentLength >= 4)
				fourOfAKind = currentCardValue;
			else
			{
				if ((highestCard == Value::Invalid) || (static_cast<int>(highestCard) < static_cast<int>(currentCardValue)))
					highestCard = currentCardValue;
			}
			currentLength = 1;
			currentCardValue = orderedCards[currentCard];
		}
	}
	if (currentLength >= 4)
		fourOfAKind = currentCardValue;
	else
	{
		if ((highestCard == Value::Invalid) || (static_cast<int>(highestCard) < static_cast<int>(currentCardValue)))
			highestCard = currentCardValue;
	}
	if ((fourOfAKind != Value::Invalid) && (highestCard != Value::Invalid))
	{
		handValue += this->computeCardValue(highestCard, 0);
		handValue += this->computeCardValue(fourOfAKind, 1);
		return true;
	}
	return false;
}

bool TexasHoldem::isFullHouse(const std::vector<Value>& orderedCards, uint64_t& handValue)
{
	int currentLength = 0;
	bool hasThreeOfAKind = false;
	bool hasPair = false;
	Value currentCardValue = orderedCards[0];
	Value threeOfAKindValue = Value::Invalid;
	Value pairValue = Value::Invalid;
	for (int currentCard = 0; currentCard < orderedCards.size(); ++currentCard)
	{
		if (currentCardValue == orderedCards[currentCard])
			++currentLength;
		else
		{
			this->updateFullHouse(currentLength, hasPair, hasThreeOfAKind, threeOfAKindValue, pairValue, currentCardValue);
			currentLength = 1;
			currentCardValue = orderedCards[currentCard];
		}
	}
	this->updateFullHouse(currentLength, hasPair, hasThreeOfAKind, threeOfAKindValue, pairValue, currentCardValue);
	if (hasThreeOfAKind && hasPair)
	{
		handValue += this->computeCardValue(pairValue, 0);
		handValue += this->computeCardValue(threeOfAKindValue, 1);
	}
	return hasThreeOfAKind && hasPair;
}

bool TexasHoldem::isFlush(const std::vector<Value>& filteredBySuitAndOrdered, uint64_t& handValue)
{
	bool containsAce = (filteredBySuitAndOrdered[0] == Value::Ace);
	if (containsAce)
		handValue += this->computeCardValue(Value::Ace, 0);
	int numberOfCardsToGet = containsAce ? 4 : 5;
	for (int cardToGet = 0; cardToGet < numberOfCardsToGet; ++cardToGet)
	{
		handValue += this->computeCardValue(filteredBySuitAndOrdered[filteredBySuitAndOrdered.size() - 1 - cardToGet], 0);
	}
	return true;
}

void TexasHoldem::updateFullHouse(int currentLength, bool& hasPair, bool& hasThreeOfAKind, Value& threeOfAKindValue,
								  Value& pairValue, Value& currentCardValue)
{
	if (currentLength >= 3)
	{
		if (!hasThreeOfAKind)
		{
			hasThreeOfAKind = true;
			threeOfAKindValue = currentCardValue;
		}
		else
		{
			if (CardEvaluation::leftCardHasGreaterValue(currentCardValue, threeOfAKindValue))
			{
				hasPair = true;
				pairValue = (hasPair && CardEvaluation::leftCardHasGreaterValue(pairValue, threeOfAKindValue)) ? pairValue : threeOfAKindValue;
				threeOfAKindValue = currentCardValue;
			}
			else
			{
				hasPair = true;
				pairValue = (hasPair && CardEvaluation::leftCardHasGreaterValue(pairValue, currentCardValue)) ? pairValue : currentCardValue;
			}
		}
	}
	else if (currentLength >= 2)
	{
		hasPair = true;
		pairValue = (hasPair && CardEvaluation::leftCardHasGreaterValue(pairValue, currentCardValue)) ? pairValue : currentCardValue;
	}
}

bool TexasHoldem::isThreeOfAKind(const std::vector<Value>& orderedCards, uint64_t& handValue)
{
	int currentLength = 0;
	bool hasThreeOfAKind = false;
	Value currentCardValue = orderedCards[0];
	Value threeOfAKindValue = Value::Invalid;
	Value highCardValue = Value::Invalid;
	Value secondHighCardValue = Value::Invalid;
	for (int currentCard = 0; currentCard < orderedCards.size(); ++currentCard)
	{
		if (currentCardValue == orderedCards[currentCard])
			++currentLength;
		else
		{
			if (currentLength >= 3)
			{
				hasThreeOfAKind = true;
				threeOfAKindValue = currentCardValue;
			}
			else
			{
				if (highCardValue == Value::Invalid || CardEvaluation::leftCardHasGreaterValue(currentCardValue, highCardValue))
				{
					secondHighCardValue = highCardValue;
					highCardValue = currentCardValue;
				}
				else if (secondHighCardValue == Value::Invalid || CardEvaluation::leftCardHasGreaterValue(currentCardValue, secondHighCardValue))
				{
					secondHighCardValue = currentCardValue;
				}
			}
			currentLength = 1;
			currentCardValue = orderedCards[currentCard];
		}
	}
	if (currentLength >= 3)
	{
		hasThreeOfAKind = true;
		threeOfAKindValue = currentCardValue;
	}
	else
	{
		if (highCardValue == Value::Invalid || CardEvaluation::leftCardHasGreaterValue(currentCardValue, highCardValue))
		{
			secondHighCardValue = highCardValue;
			highCardValue = currentCardValue;
		}
		else if (secondHighCardValue == Value::Invalid || CardEvaluation::leftCardHasGreaterValue(currentCardValue, secondHighCardValue))
		{
			secondHighCardValue = currentCardValue;
		}
	}
	if (hasThreeOfAKind)
	{
		handValue += this->computeCardValue(threeOfAKindValue, 1);
		handValue += this->computeCardValue(highCardValue, 0);
		handValue += this->computeCardValue(secondHighCardValue, 0);
	}
	return hasThreeOfAKind;
}

bool TexasHoldem::isTwoPair(const std::vector<Value>& orderedCards, uint64_t& handValue)
{
	int currentLength = 0;
	bool previousPair = false;
	Value highPairValue = Value::Invalid;
	Value lowPairValue = Value::Invalid;
	Value highCardValue = Value::Invalid;
	Value currentCardValue = orderedCards[0];
	for (int currentCard = 0; currentCard < orderedCards.size(); ++currentCard)
	{
		if (currentCardValue == orderedCards[currentCard])
			++currentLength;
		else
		{
			if (currentLength >= 2)
			{
				if (!previousPair || CardEvaluation::leftCardHasGreaterValue(currentCardValue, highPairValue))
				{
					previousPair = true;
					lowPairValue = highPairValue;
					highPairValue = currentCardValue;
				}
				else if (CardEvaluation::leftCardHasGreaterValue(currentCardValue, lowPairValue))
				{
					highCardValue = (CardEvaluation::leftCardHasGreaterValue(lowPairValue, highCardValue)) ? lowPairValue : highCardValue;
					lowPairValue = currentCardValue;
				}
			}
			else
			{
				if (CardEvaluation::leftCardHasGreaterValue(currentCardValue, highCardValue))
				{
					highCardValue = currentCardValue;
				}
			}
			currentLength = 1;
			currentCardValue = orderedCards[currentCard];
		}
	}
	if (currentLength >= 2)
	{
		if (!previousPair || CardEvaluation::leftCardHasGreaterValue(currentCardValue, highPairValue))
		{
			previousPair = true;
			lowPairValue = highPairValue;
			highPairValue = currentCardValue;
		}
		else if (CardEvaluation::leftCardHasGreaterValue(currentCardValue, lowPairValue))
		{
			highCardValue = (CardEvaluation::leftCardHasGreaterValue(lowPairValue, highCardValue)) ? lowPairValue : highCardValue;
			lowPairValue = currentCardValue;
		}
	}
	else
	{
		if (CardEvaluation::leftCardHasGreaterValue(currentCardValue, highCardValue))
		{
			highCardValue = currentCardValue;
		}
	}
	if ((highPairValue != Value::Invalid) && (lowPairValue != Value::Invalid) && (highCardValue != Value::Invalid))
	{
		handValue += this->computeCardValue(highPairValue, 1);
		handValue += this->computeCardValue(lowPairValue, 1);
		handValue += this->computeCardValue(highCardValue, 0);
		return true;
	}

	return false;
}

bool TexasHoldem::isPair(const std::vector<Value>& orderedCards, uint64_t& handValue)
{
	int currentLength = 0;
	Value currentCardValue = orderedCards[0];
	Value pairValue = Value::Invalid;
	Value highCardValue = Value::Invalid;
	Value midCardValue = Value::Invalid;
	Value lowCardValue = Value::Invalid;
	for (int currentCard = 0; currentCard < orderedCards.size(); ++currentCard)
	{
		if (currentCardValue == orderedCards[currentCard])
			++currentLength;
		else
		{
			if (currentLength >= 2)
			{
				pairValue = currentCardValue;
			}
			else 
			{
				if ((highCardValue == Value::Invalid) || CardEvaluation::leftCardHasGreaterValue(currentCardValue, highCardValue))
				{
					lowCardValue = midCardValue;
					midCardValue = highCardValue;
					highCardValue = currentCardValue;
				}
				else if ((midCardValue == Value::Invalid) || CardEvaluation::leftCardHasGreaterValue(currentCardValue, midCardValue))
				{
					lowCardValue = midCardValue;
					midCardValue = currentCardValue;	
				}
				else if ((lowCardValue == Value::Invalid) || CardEvaluation::leftCardHasGreaterValue(currentCardValue, lowCardValue))
				{
					lowCardValue = currentCardValue;
				}
			}
			currentLength = 1;
			currentCardValue = orderedCards[currentCard];
		}
	}
	if (currentLength >= 2)
	{
		pairValue = currentCardValue;
	}
	else
	{
		if ((highCardValue == Value::Invalid) || CardEvaluation::leftCardHasGreaterValue(currentCardValue, highCardValue))
		{
			lowCardValue = midCardValue;
			midCardValue = highCardValue;
			highCardValue = currentCardValue;
		}
		else if ((midCardValue == Value::Invalid) || CardEvaluation::leftCardHasGreaterValue(currentCardValue, midCardValue))
		{
			lowCardValue = midCardValue;
			midCardValue = currentCardValue;
		}
		else if ((lowCardValue == Value::Invalid) || CardEvaluation::leftCardHasGreaterValue(currentCardValue, lowCardValue))
		{
			lowCardValue = currentCardValue;
		}
	}
	if ((pairValue != Value::Invalid) && (lowCardValue != Value::Invalid) && (midCardValue != Value::Invalid) && (highCardValue != Value::Invalid))
	{
		handValue += this->computeCardValue(pairValue, 1);
		handValue += this->computeCardValue(highCardValue, 0);
		handValue += this->computeCardValue(midCardValue, 0);
		handValue += this->computeCardValue(lowCardValue, 0);

		return true;
	}
	return false;
}

bool TexasHoldem::isHighCard(const std::vector<Value>& orderedCards, uint64_t& handValue)
{
	bool containsAce = (orderedCards[0] == Value::Ace);
	if (containsAce)
		handValue += this->computeCardValue(Value::Ace, 0);
	int numberOfCardsToGet = containsAce ? 4 : 5;
	for (int cardToGet = 0; cardToGet < numberOfCardsToGet; ++cardToGet)
	{
		handValue += this->computeCardValue(orderedCards[orderedCards.size() - 1 - cardToGet], 0);
	}
	return true;
}

void TexasHoldem::setSharedCards(std::array<Card, 5>& sharedCards)
{
	this->sharedCards = sharedCards;
}


std::vector<int> TexasHoldem::determineWinner()
{
	assert(this->currentGameState == GameState::River);

	std::vector<int> winningIndex{};

	if (this->onlyOnePlayerLeft())
	{
		// Get index of the surviving player
		for (int i = 0; i < this->playerCurrentlyPlaying.size(); i++)
		{
			if (this->playerCurrentlyPlaying[i])
			{
				winningIndex = { i };
				break;
			}
		}
	}
	else
	{
		uint64_t winningHandValue = 0;
		int currentIndex = 0;
		for (auto player : this->players)
		{
			HandValue winningValue = HandValue::HighCard;
			if (!this->playerCurrentlyPlaying.at(currentIndex))
				continue;
			uint64_t handValue = 0;
			HandValue playerHandValue = this->evaluateHand((*player).getHand(), handValue);
			assert(playerHandValue != HandValue::Invalid);
			if (static_cast<int>(playerHandValue) > static_cast<int>(winningValue))
			{
				winningValue = playerHandValue;
				winningHandValue = handValue;
				winningIndex = { currentIndex };
			}
			else if (static_cast<int>(playerHandValue) == static_cast<int>(winningValue))
			{
				if (handValue > winningHandValue)
				{
					winningValue = playerHandValue;
					winningHandValue = handValue;
					winningIndex = { currentIndex };
				}
				else if (handValue == winningHandValue)
					winningIndex.push_back(currentIndex);
			}
			currentIndex++;
		}
		for (int index : winningIndex)
			assert(index >= 0);
		assert(winningIndex.size() > 0);
	}
	return winningIndex;
}

void TexasHoldem::assignEarningsToWinner(std::vector<int> winningPlayers)
{
	assert(winningPlayers.size() > 0);
	int totalEarningsPerPlayer = this->currentTotalBetAmount / winningPlayers.size();
	for (int playerIndex = 0; playerIndex < this->numPlayers; ++playerIndex)
	{
		// Find playerIdx in winningPlayers
		if(std::find(winningPlayers.begin(), winningPlayers.end(), playerIndex) != winningPlayers.end())
			(*this->players.at(playerIndex)).addPlayerEarnings(totalEarningsPerPlayer, this->startingStack);
		else
			(*this->players.at(playerIndex)).addPlayerEarnings(0, this->startingStack);
	}
}

HandValue TexasHoldem::evaluateHand(Hand& hand, uint64_t& handValue)
{
	// Vectors to save the list of ordered cards for each suit
	std::vector<std::vector<Value>> filteredBySuitAndOrdered{ {} , {}, {}, {} };
	std::vector<Value> orderedByValue{};

	// Loop over the hand and the shared cards and filter them by suit
	for (auto card : hand.getHand())
	{
		assert(card.first != Suit::Invalid);
		assert(card.second != Value::Invalid);
		int index = static_cast<int>(card.first);
		filteredBySuitAndOrdered[index].push_back(card.second);
		orderedByValue.push_back(card.second);
	}
	for (auto card : this->sharedCards)
	{
		assert(card.first != Suit::Invalid);
		assert(card.second != Value::Invalid);
		if (card.first == Suit::Invalid || card.second == Value::Invalid)
			return HandValue::Invalid;
		int index = static_cast<int>(card.first);
		filteredBySuitAndOrdered[index].push_back(card.second);
		orderedByValue.push_back(card.second);
	}

	// Check if any of the vectors has 5 or more elements
	int possibleStraightFlush = -1;
	for (int suit = 0; suit < 4; ++suit)
	{
		if (filteredBySuitAndOrdered[suit].size() >= 5)
		{
			possibleStraightFlush = suit;
			break;
		}
	}

	// If there is a possible Straight Flush we check for them
	if (possibleStraightFlush != -1)
	{
		// Order the vector with the possible straight flush
		std::vector<Value> possibleStraightFlushValues = filteredBySuitAndOrdered[possibleStraightFlush];
		std::sort(possibleStraightFlushValues.begin(), possibleStraightFlushValues.end(), CardEvaluation::orderCardValue);
		if (this->isRoyalFlush(possibleStraightFlushValues, handValue))
			return HandValue::RoyalFlush;
		if (this->isStraightFlush(possibleStraightFlushValues, handValue))
			return HandValue::StraightFlush;

		this->isFlush(possibleStraightFlushValues, handValue);
		return HandValue::Flush;
	}
	
	// Order the vector by it's value
	std::sort(orderedByValue.begin(), orderedByValue.end(), CardEvaluation::orderCardValue);
	if (this->isFourOfAKind(orderedByValue, handValue))
		return HandValue::FourOfAKind;
	if (this->isFullHouse(orderedByValue, handValue))
		return HandValue::FullHouse;
	if (this->isStraight(orderedByValue, handValue))
		return HandValue::Straight;
	if (this->isThreeOfAKind(orderedByValue, handValue))
		return HandValue::ThreeOfAKind;
	if (this->isTwoPair(orderedByValue, handValue))
		return HandValue::TwoPair;
	if (this->isPair(orderedByValue, handValue))
		return HandValue::Pair;

	this->isHighCard(orderedByValue, handValue);
	return HandValue::HighCard;
}

void TexasHoldem::playRound()
{
	this->beginRound();
	this->dealCards();
	this->bettingRound();
	this->drawFlop();
	this->bettingRound();
	this->drawTurn();
	this->bettingRound();
	this->drawRiver();
	this->bettingRound();
	
	std::vector<int> winnerIndex = this->determineWinner();
	this->assignEarningsToWinner(winnerIndex);
	this->endRound();
}

void TexasHoldem::playMultipleRounds(int numberOfRounds)
{
	for (int round = 0; round < numberOfRounds; ++round)
		this->playRound();
}

bool TexasHoldem::allPlayersAllIn()
{
	// Return true if all players have all in, false otherwise
	for (int i = 0; i < this->players.size(); i++)
	{
		if (this->playerCurrentlyPlaying.at(i) && (*this->players[i]).getStack() > 0.0f)
		{
			return false;
		}
	}
	return true;
	
}

bool TexasHoldem::onlyOnePlayerLeft()
{
	return std::count(this->playerCurrentlyPlaying.begin(), this->playerCurrentlyPlaying.end(), true) == 1;
}

void TexasHoldem::bettingRound()
{
	if (this->allPlayersAllIn())
		return;
	int betStartingPosition, lastBetPosition;
	std::vector<int> currentPlayerBets(this->numPlayers, 0);
	if (this->currentGameState == GameState::PreFlop)
	{
		betStartingPosition = (this->dealerPosition + 3) % (this->numPlayers);
		int smallBlindPosition = (this->dealerPosition + 1) % (this->numPlayers);
		int bigBlindPosition = (this->dealerPosition + 2) % (this->numPlayers);

		if ((*this->players.at(smallBlindPosition)).getStack() < this->bigBlind)
		{
			this->playerCurrentlyPlaying.at(smallBlindPosition) = false;
			this->currentlyPlayingPlayers--;
		}
		else
		{
			//currentPlayerBets.at((this->dealerPosition + 1) % (this->numPlayers)) = this->smallBlind;
			(*this->players.at(smallBlindPosition)).decreaseStackSize(this->smallBlind);
			this->currentTotalBetAmount += this->smallBlind;
		}

		if ((*this->players.at(bigBlindPosition)).getStack() < this->bigBlind)
		{
			this->playerCurrentlyPlaying.at(bigBlindPosition) = false;
			this->currentlyPlayingPlayers--;
		}			
		else
		{
			//currentPlayerBets.at((this->dealerPosition + 2) % (this->numPlayers)) = this->bigBlind;
			(*this->players.at(bigBlindPosition)).decreaseStackSize(this->bigBlind);
			this->currentTotalBetAmount += this->bigBlind;
		}
			
	}
	else
		betStartingPosition = (this->dealerPosition - 1 + this->numPlayers) % (this->numPlayers);
	int soFarBetValue = this->bigBlind;
		
	lastBetPosition = (betStartingPosition - 1 + this->numPlayers) % (this->numPlayers);
	int currentPosition = betStartingPosition;

	bool firstToBet = true;

	while (true)
	{
		if (this->currentlyPlayingPlayers == 1)
			break;
		if (this->playerCurrentlyPlaying.at(currentPosition))
		{
			State state = this->getStateOfPlayer(currentPosition);
			Decision currentPlayerDecision = (*this->players.at(currentPosition)).decide(static_cast<int>(this->currentGameState), state, this->smallBlind, soFarBetValue);
			
			//std::cout << "Idx: " << currentPosition << " " <<  static_cast<int>(currentPlayerDecision.play) << " " << currentPlayerDecision.betAmount << std::endl;

			if (currentPlayerDecision.play == Play::Raise)
			{
				assert(currentPlayerDecision.betAmount + soFarBetValue <= (*this->players.at(currentPosition)).getStack());

				soFarBetValue += currentPlayerDecision.betAmount;
				this->lastBet = currentPlayerDecision.betAmount;
				currentPlayerBets.at(currentPosition) = soFarBetValue;
				lastBetPosition = (currentPosition - 1 + this->numPlayers) % (this->numPlayers);
			}
			else if (currentPlayerDecision.play == Play::Call)
			{
				assert(soFarBetValue <= (*this->players.at(currentPosition)).getStack());

				//currentPlayerBets.at(currentPosition) += currentBetValue;
				this->lastBet = 0;
				currentPlayerBets.at(currentPosition) = soFarBetValue;
				if (lastBetPosition == currentPosition)
					break;
			}
			else
			{
				--this->currentlyPlayingPlayers;
				this->lastBet = 0;
				this->playerCurrentlyPlaying.at(currentPosition) = false;
				if (lastBetPosition == currentPosition)
					break;
			}
		}
		else
		{
			if (lastBetPosition == currentPosition)
				break;
		}
		++currentPosition;
		currentPosition = (currentPosition % this->numPlayers);
	}
	this->lastBet = 0;
	int betIndex = 0;
	for (int bet : currentPlayerBets)
	{
		assert(bet >= 0);
		this->currentTotalBetAmount += bet;
		assert(bet <= (*this->players.at(betIndex)).getStack());
		(*this->players.at(betIndex)).decreaseStackSize(bet);
		++betIndex;
	}
}

State TexasHoldem::getStateOfPlayer(int index) {
	State state = { {} };
	this->addLastBetDelta(state);
	this->addMoneyLeftPlayer(state, index);
	this->addMoneyLeftOponent(state, index);
	this->addPlayerBetAmount(state, index);
	this->addCurrentHandCards(state, index);
	this->addCurrentHandSuits(state, index);
	return state;
}

void TexasHoldem::addLastBetDelta(State& state)
{
	state.values[0] = this->lastBet;
}

void TexasHoldem::addMoneyLeftPlayer(State& state, int playerIndex)
{
	int moneyLeft = this->players.at(playerIndex)->getStack();
	state.values[1] = (float)moneyLeft / (float)this->startingStack;
}

void TexasHoldem::addMoneyLeftOponent(State& state, int playerIndex)
{
	int moneyLeft = this->players.at((playerIndex - 1 + this->numPlayers) % this->numPlayers)->getStack();
	state.values[2] = (float)moneyLeft / (float)this->startingStack;
}

void TexasHoldem::addPlayerBetAmount(State& state, int playerIndex)
{
	state.values[3] = (float)this->currentTotalBetAmount / (float)this->startingStack;
}

void TexasHoldem::addCurrentHandCards(State& state, int playerIndex)
{
	Hand hand = this->players.at(playerIndex)->getHand();
	int numberOfCards[13] = {};
	for (Card card : hand.getHand())
	{
		numberOfCards[static_cast<int>(card.second)] += 1;
	}
	for (Card card : this->sharedCards)
	{
		int cardNumber = static_cast<int>(card.second);
		if (cardNumber == 13)
			break;
		numberOfCards[cardNumber] += 1;
	}
	for (int cardIndex = 0; cardIndex < 13; ++cardIndex)
	{
		state.values[4 + cardIndex] = numberOfCards[cardIndex];
	}
}

void TexasHoldem::addCurrentHandSuits(State& state, int playerIndex)
{
	Hand hand = this->players.at(playerIndex)->getHand();
	int numberOfSuits[4] = {};
	for (Card card : hand.getHand())
	{
		numberOfSuits[static_cast<int>(card.first)] += 1;
	}
	for (Card card : this->sharedCards)
	{
		int suitNumber = static_cast<int>(card.first);
		if (suitNumber == 4)
			break;
		numberOfSuits[suitNumber] += 1;
	}
	for (int cardIndex = 0; cardIndex < 4; ++cardIndex)
	{
		state.values[17 + cardIndex] = numberOfSuits[cardIndex];
	}
}
