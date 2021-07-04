#include "texas-holdem.h"
#include "deck.h"
#include "player.h"
#include <algorithm>
#include <vector>

TexasHoldem::TexasHoldem(int numPlayers, float startingStack, Agent& decisionAgent, Deck& deck, 
	StraightIdentifier& straightIdentifier): currentDeck(deck), straightIdentifier(straightIdentifier)
{
	this->dealerPosition = 0;
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
	for (int cardNumber = 0; cardNumber < 3; ++cardNumber)
		sharedCards[cardNumber] = this->getNextCard();
}

void TexasHoldem::drawTurn()
{
	sharedCards[3] = this->getNextCard();
}

void TexasHoldem::drawRiver()
{
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
}

bool TexasHoldem::isRoyalFlush(const std::vector<Value>& filteredBySuitAndOrdered)
{
	return this->straightIdentifier.hasRoyalStraight(filteredBySuitAndOrdered);
}

bool TexasHoldem::isStraightFlush(const std::vector<Value>& filteredBySuitAndOrdered)
{
	return this->straightIdentifier.hasStraightFlush(filteredBySuitAndOrdered);
}

bool TexasHoldem::isStraight(const std::vector<Value>& orderedCards)
{
	return this->straightIdentifier.hasStraight(orderedCards);
}

bool TexasHoldem::isFourOfAKind(const std::vector<Value>& orderedCards)
{
	int currentLength = 0;
	Value currentCardValue = orderedCards[0];
	for (int currentCard = 0; currentCard < orderedCards.size(); ++currentCard)
	{
		if (currentCardValue == orderedCards[currentCard])
			++currentLength;
		else
		{
			currentLength = 1;
			currentCardValue = orderedCards[currentCard];
		}
		if (currentLength >= 4)
			return true;
	}
	return false;
}

bool TexasHoldem::isFullHouse(const std::vector<Value>& orderedCards)
{
	int currentLength = 0;
	bool hasThreeOfAKind = false;
	bool hasPair = false;
	Value currentCardValue = orderedCards[0];
	for (int currentCard = 0; currentCard < orderedCards.size(); ++currentCard)
	{
		if (currentCardValue == orderedCards[currentCard])
			++currentLength;
		else
		{
			if (currentLength >= 3 && !hasThreeOfAKind)
				hasThreeOfAKind = true;
			else if (currentLength >= 2)
				hasPair = true;
			currentLength = 1;
			currentCardValue = orderedCards[currentCard];
		}
	}
	if (currentLength >= 3 && !hasThreeOfAKind)
		hasThreeOfAKind = true;
	else if (currentLength >= 2)
		hasPair = true;
	return hasThreeOfAKind && hasPair;
}

bool TexasHoldem::isThreeOfAKind(const std::vector<Value>& orderedCards)
{
	int currentLength = 0;
	Value currentCardValue = orderedCards[0];
	for (int currentCard = 0; currentCard < orderedCards.size(); ++currentCard)
	{
		if (currentCardValue == orderedCards[currentCard])
			++currentLength;
		else
		{
			if (currentLength >= 3)
				return true;
			else
			{
				currentLength = 1;
				currentCardValue = orderedCards[currentCard];
			}
		}
	}
	return currentLength >= 3;
}

bool TexasHoldem::isTwoPair(const std::vector<Value>& orderedCards)
{
	int currentLength = 0;
	bool previousPair = false;
	Value currentCardValue = orderedCards[0];
	for (int currentCard = 0; currentCard < orderedCards.size(); ++currentCard)
	{
		if (currentCardValue == orderedCards[currentCard])
			++currentLength;
		else
		{
			if ((currentLength >= 2) && previousPair)
				return true;
			else if (currentLength >= 2)
				previousPair = true;
			currentLength = 1;
			currentCardValue = orderedCards[currentCard];
		}
	}
	return ((currentLength >= 2) && previousPair);
}

bool TexasHoldem::isPair(const std::vector<Value>& orderedCards)
{
	int currentLength = 0;
	Value currentCardValue = orderedCards[0];
	for (int currentCard = 0; currentCard < orderedCards.size(); ++currentCard)
	{
		if (currentCardValue == orderedCards[currentCard])
			++currentLength;
		else
		{
			if (currentLength >= 2)
				return true;
			currentLength = 1;
			currentCardValue = orderedCards[currentCard];
		}
	}
	return (currentLength >= 2);
}

void TexasHoldem::setSharedCards(std::array<Card, 5>& sharedCards)
{
	this->sharedCards = sharedCards;
}

HandValue TexasHoldem::evaluateHand(Hand& hand)
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
		if (this->isRoyalFlush(possibleStraightFlushValues))
			return HandValue::RoyalFlush;
		if (this->isStraightFlush(possibleStraightFlushValues))
			return HandValue::StraightFlush;

		return HandValue::Flush;
	}
	
	// Order the vector by it's value
	std::sort(orderedByValue.begin(), orderedByValue.end(), CardEvaluation::orderCardValue);
	if (this->isFourOfAKind(orderedByValue))
		return HandValue::FourOfAKind;
	if (this->isFullHouse(orderedByValue))
		return HandValue::FullHouse;
	if (this->isStraight(orderedByValue))
		return HandValue::Straight;
	if (this->isThreeOfAKind(orderedByValue))
		return HandValue::ThreeOfAKind;
	if (this->isTwoPair(orderedByValue))
		return HandValue::TwoPair;
	if (this->isPair(orderedByValue))
		return HandValue::Pair;

	return HandValue::HighCard;
}
