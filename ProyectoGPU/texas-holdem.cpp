#include "texas-holdem.h"
#include "deck.h"
#include "player.h"
#include <vector>
#include <cstdint>

TexasHoldem::TexasHoldem(int numPlayers, float startingStack, Agent& decisionAgent, Deck& deck, 
	StraightIdentifier& straightIdentifier): currentDeck(deck), straightIdentifier(straightIdentifier)
{
	this->dealerPosition = 0;
	this->currentGameState = GameState::Invalid;
	this->numPlayers = numPlayers;
	this->sharedCards = { invalidCard, invalidCard, invalidCard, invalidCard, invalidCard };
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

void TexasHoldem::dealCards()
{
	this->currentGameState = GameState::PreFlop;
	// We give 2 cards to each player
	for (int cardNumber = 0; cardNumber < 2; ++cardNumber)
	{
		for (auto player : this->players)
		{
			player.addCardToHand(this->getNextCard(), cardNumber);
		}
	}
}

void TexasHoldem::drawFlop()
{
	this->currentGameState = GameState::Flop;
	for (int cardNumber = 0; cardNumber < 3; ++cardNumber)
		sharedCards[cardNumber] = this->getNextCard();
}

void TexasHoldem::drawTurn()
{
	this->currentGameState = GameState::Turn;
	sharedCards[3] = this->getNextCard();
}

void TexasHoldem::drawRiver()
{
	this->currentGameState = GameState::River;
	sharedCards[4] = this->getNextCard();
}

void TexasHoldem::resetSharedCards()
{
	this->sharedCards = { invalidCard, invalidCard, invalidCard, invalidCard, invalidCard };
}

void TexasHoldem::resetPlayerCards()
{
	for (auto player : this->players)
		player.resetHand();
}

void TexasHoldem::endRound()
{
	this->resetDeck();
	this->resetPlayerCards();
	this->resetSharedCards();
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


int TexasHoldem::determineWinner()
{
	HandValue winningValue = HandValue::HighCard;
	if (this->currentGameState == GameState::River)
	{
		for (auto player : this->players)
		{
			uint64_t handValue;
			HandValue playerHandValue = this->evaluateHand(player.getHand(), handValue);
			if (static_cast<int>(playerHandValue) > static_cast<int>(winningValue))
				winningValue = playerHandValue;
		}
		// TODO: Get the winning player by comparing the hand value, and if the hand value is the same
		// we need a way to compare the hands for the players.
	}
	else
		return -1;
}

HandValue TexasHoldem::evaluateHand(Hand& hand, uint64_t& handValue)
{
	// Vectors to save the list of ordered cards for each suit
	std::vector<std::vector<Value>> filteredBySuitAndOrdered{ {} , {}, {}, {} };
	std::vector<Value> orderedByValue{};

	// Loop over the hand and the shared cards and filter them by suit
	for (auto card : hand.getHand())
	{
		if (card.first == Suit::Invalid || card.second == Value::Invalid)
			return HandValue::Invalid;
		int index = static_cast<int>(card.first);
		filteredBySuitAndOrdered[index].push_back(card.second);
		orderedByValue.push_back(card.second);
	}
	for (auto card : this->sharedCards)
	{
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
