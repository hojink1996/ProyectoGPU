#include <array>
#include "pch.h"
#include "CppUnitTest.h"
#include "../ProyectoGPU/deck.h"
#include "../ProyectoGPU/deck.cpp"
#include "../ProyectoGPU/card.h"
#include "../ProyectoGPU/card.cpp"
#include "../ProyectoGPU/hand.h"
#include "../ProyectoGPU/hand.cpp"
#include "../ProyectoGPU/player.h"
#include "../ProyectoGPU/player.cpp"
#include "../ProyectoGPU/individual.h"
#include "../ProyectoGPU/individual.cpp"
#include "../ProyectoGPU/straight-identifier.h"
#include "../ProyectoGPU/straight-identifier.cpp"
#include "../ProyectoGPU/texas-holdem.h"
#include "../ProyectoGPU/texas-holdem.cpp"
#include "../ProyectoGPU/agent.h"
#include "../ProyectoGPU/agent.cpp"
#include "../ProyectoGPU/genetic-algorithm.h"
#include "../ProyectoGPU/genetic-algorithm.cpp"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(TestDeck)
{
public:
	TEST_METHOD(TestCreation)
	{
		// Make a band new card and draw the first card (should be the ace of clubs)
		Deck testDeck = Deck();
		Card firstCard = testDeck.drawCard();
		Card expectedCard = std::make_pair(Suit::Clubs, Value::Ace);

		// Verify that we got the expected card
		Assert::AreEqual(static_cast<int>(expectedCard.first), static_cast<int>(firstCard.first));
		Assert::AreEqual(static_cast<int>(expectedCard.second), static_cast<int>(firstCard.second));
	}

	TEST_METHOD(TestDrawingCards)
	{
		// Make a brand new deck of cards and draw two cards
		Deck testDeck = Deck();
		Card firstCard = testDeck.drawCard();
		Card secondCard = testDeck.drawCard();

		// Get the output value for the card
		int firstValue = 13 * static_cast<int>(firstCard.first) + static_cast<int>(firstCard.second);
		int secondValue = 13 * static_cast<int>(secondCard.first) + static_cast<int>(secondCard.second);

		// Test that the first and second values are not the same
		Assert::AreNotEqual(firstValue, secondValue);
	}

	TEST_METHOD(TestShuffling)
	{
		// Make a brand new deck of cards and shuffle it
		Deck testDeck = Deck();
		testDeck.reset();

		// Draw the first card
		Card firstCard = testDeck.drawCard();

		// Compare with the first card in an initially unshuffled deck (note that this test
		// may fail 1/52th of the time)
		bool suitVerification = (0 == static_cast<int>(firstCard.first));
		bool valueVerification = (0 == static_cast<int>(firstCard.second));
		Assert::IsFalse(suitVerification && valueVerification);
	}
};

TEST_CLASS(TestHand)
{
public:
	TEST_METHOD(TestCreation)
	{
		// Create a hand and verify that we have the default values during creation (invalid cards)
		Hand hand = Hand();
		std::array<Card, 2> initialCards = hand.getHand();
		Suit invalidSuit = Suit::Invalid;
		Value invalidValue = Value::Invalid;

		// Make sure that the cards in the hand are invalid
		Assert::AreEqual(static_cast<int>(invalidSuit), static_cast<int>(initialCards[0].first));
		Assert::AreEqual(static_cast<int>(invalidValue), static_cast<int>(initialCards[0].second));
		Assert::AreEqual(static_cast<int>(invalidSuit), static_cast<int>(initialCards[1].first));
		Assert::AreEqual(static_cast<int>(invalidValue), static_cast<int>(initialCards[1].second));
	}
	TEST_METHOD(TestDrawingCard)
	{
		// Create a hand, give it a card from the deck and then check that it works as intended
		Hand hand = Hand();
		Deck deck = Deck();
		deck.reset();

		// Draw the two cards
		Card firstCard = deck.drawCard();
		Card secondCard = deck.drawCard();

		// Add the cards to the hand
		hand.addCardToHand(firstCard, 0);
		hand.addCardToHand(secondCard, 1);

		// Invalid values (the cards should not be invalid values)
		Suit invalidSuit = Suit::Invalid;
		Value invalidValue = Value::Invalid;

		// Check that the cards were added to the hand correctly
		Assert::AreNotEqual(static_cast<int>(invalidSuit), static_cast<int>(hand.getHand()[0].first));
		Assert::AreNotEqual(static_cast<int>(invalidValue), static_cast<int>(hand.getHand()[0].second));
		Assert::AreNotEqual(static_cast<int>(invalidSuit), static_cast<int>(hand.getHand()[1].first));
		Assert::AreNotEqual(static_cast<int>(invalidValue), static_cast<int>(hand.getHand()[1].second));

		// Reset the hand afterwards
		hand.resetHand();

		// Verify that the hand was reset correctly
		Assert::AreEqual(static_cast<int>(invalidSuit), static_cast<int>(hand.getHand()[0].first));
		Assert::AreEqual(static_cast<int>(invalidValue), static_cast<int>(hand.getHand()[0].second));
		Assert::AreEqual(static_cast<int>(invalidSuit), static_cast<int>(hand.getHand()[1].first));
		Assert::AreEqual(static_cast<int>(invalidValue), static_cast<int>(hand.getHand()[1].second));
	}
};

TEST_CLASS(TestStraightIdentifier)
{
public:
	TEST_METHOD(TestRoyalFlush)
	{
		// Create two positive and negative examples
		StraightIdentifier straightIdentifier = StraightIdentifier();
		std::vector<Value> basicRoyalStraight{ Value::Ace, Value::Ten, Value::Jack, Value::Queen, Value::King };
		std::vector<Value> royalStraightWithExtra{ Value::Ace, Value::Five, Value::Ten, Value::Jack, Value::Queen, Value::King };
		std::vector<Value> notRoyalStraight{ Value::Ace, Value::Five, Value::Ten, Value::Jack, Value::Queen};
		std::vector<Value> isStraightButNotRoyalStraight{ Value::Ace, Value::Two, Value::Three, Value::Four, Value::Five };

		// Check the output
		Value highCard = Value::Invalid;
		Assert::IsTrue(straightIdentifier.hasRoyalStraight(basicRoyalStraight, highCard));
		Assert::IsTrue(straightIdentifier.hasRoyalStraight(royalStraightWithExtra, highCard));
		Assert::IsFalse(straightIdentifier.hasRoyalStraight(notRoyalStraight, highCard));
		Assert::IsFalse(straightIdentifier.hasRoyalStraight(isStraightButNotRoyalStraight, highCard));
	}

	TEST_METHOD(TestStraightFlush)
	{
		StraightIdentifier straightIdentifier = StraightIdentifier();
		std::vector<Value> basicStraight{ Value::Ace, Value::Two, Value::Three, Value::Four, Value::Five };
		std::vector<Value> simpleMixedStraight{ Value::Ace, Value::Five, Value::Six, Value::Seven, Value::Eight, Value::Nine };
		std::vector<Value> mixedStraight{ Value::Ace, Value::Two, Value::Five, Value::Six, Value::Seven, Value::Eight, Value::Nine };
		std::vector<Value> notStraight{ Value::Ace, Value::Two, Value::Three, Value::Four, Value::Queen, Value::King };
		std::vector<Value> notStraightThrees{ Value::Three, Value::Four, Value::Five, Value::Ten, Value::Jack, Value::Queen };

		// Check the output
		Value highCard = Value::Invalid;
		Assert::IsTrue(straightIdentifier.hasStraightFlush(basicStraight, highCard));
		Assert::IsTrue(straightIdentifier.hasStraightFlush(simpleMixedStraight, highCard));
		Assert::IsTrue(straightIdentifier.hasStraightFlush(mixedStraight, highCard));
		Assert::IsFalse(straightIdentifier.hasStraightFlush(notStraight, highCard));
		Assert::IsFalse(straightIdentifier.hasStraightFlush(notStraightThrees, highCard));
	}
};

TEST_CLASS(TestTexasHoldemGame)
{
public:
	TEST_METHOD(TestRoyalFlushValidation)
	{
		// Create a game of TexasHoldem
		RandomAgent decisionAgent = RandomAgent();
		Deck deck = Deck();
		StraightIdentifier straightIdentifier = StraightIdentifier();
		TexasHoldem texasHoldemGame = TexasHoldem(1, 100.0f, decisionAgent, deck, straightIdentifier);

		// Set the cards in the shared card
		std::array<Card, 5> sharedCard{ std::make_pair(Suit::Clubs, Value::Ace), std::make_pair(Suit::Clubs, Value::Ten),
										 std::make_pair(Suit::Diamonds, Value::Ace), std::make_pair(Suit::Clubs, Value::King),
										 std::make_pair(Suit::Hearts, Value::Five) };
		std::array<Card, 5> sharedCardNotRoyal{ std::make_pair(Suit::Clubs, Value::Five), std::make_pair(Suit::Clubs, Value::Three),
												std::make_pair(Suit::Diamonds, Value::Ace), std::make_pair(Suit::Clubs, Value::King),
												std::make_pair(Suit::Hearts, Value::Five) };
		texasHoldemGame.setSharedCards(sharedCard);
		
		// Create the hand of cards
		Hand hand = Hand();
		Card firstCard = std::make_pair(Suit::Clubs, Value::Jack);
		Card secondCard = std::make_pair(Suit::Clubs, Value::Queen);
		hand.addCardToHand(firstCard, 0);
		hand.addCardToHand(secondCard, 1);

		// Check the value we get from the game
		uint64_t cardValue = 0;
		HandValue handValue =  texasHoldemGame.evaluateHand(hand, cardValue);

		// Check that we get the expected value
		Assert::AreEqual(static_cast<int>(HandValue::RoyalFlush), static_cast<int>(handValue));

		// Change to the scenario that is not a royal flush
		texasHoldemGame.setSharedCards(sharedCardNotRoyal);
		cardValue = 0;
		handValue = texasHoldemGame.evaluateHand(hand, cardValue);
		uint64_t expectedCardValue = 0b0111000001010;
		Assert::AreEqual(static_cast<int>(HandValue::Flush), static_cast<int>(handValue));
		Assert::AreEqual(cardValue, expectedCardValue);
	}
	TEST_METHOD(TestStraightFlush)
	{
		// Create a game of TexasHoldem
		RandomAgent decisionAgent = RandomAgent();
		Deck deck = Deck();
		StraightIdentifier straightIdentifier = StraightIdentifier();
		TexasHoldem texasHoldemGame = TexasHoldem(1, 100.0f, decisionAgent, deck, straightIdentifier);

		// Set the shared cards
		std::array<Card, 5> sharedCards{ std::make_pair(Suit::Diamonds, Value::Five), std::make_pair(Suit::Diamonds, Value::Nine),
										 std::make_pair(Suit::Diamonds, Value::Ace), std::make_pair(Suit::Diamonds, Value::Six),
										 std::make_pair(Suit::Hearts, Value::Five) };

		// Create the hand of cards
		Hand hand = Hand();
		Card firstCard = std::make_pair(Suit::Diamonds, Value::Seven);
		Card secondCard = std::make_pair(Suit::Diamonds, Value::Eight);
		hand.addCardToHand(firstCard, 0);
		hand.addCardToHand(secondCard, 1);

		// Check the value from the game
		texasHoldemGame.setSharedCards(sharedCards);
		uint64_t cardValue = 0;
		uint64_t expectedCardValue = 0b0000010000000;
		HandValue handValue = texasHoldemGame.evaluateHand(hand, cardValue);
		Assert::AreEqual(static_cast<int>(HandValue::StraightFlush), static_cast<int>(handValue));
		Assert::AreEqual(cardValue, expectedCardValue);
		hand.resetHand();

		// Check the case for an incorrect hand
		firstCard = std::make_pair(Suit::Clubs, Value::Seven);
		secondCard = std::make_pair(Suit::Clubs, Value::Eight);
		hand.addCardToHand(firstCard, 0);
		hand.addCardToHand(secondCard, 1);

		// Check that we don't get a straight flush in this case
		cardValue = 0;
		expectedCardValue = 0b0000010000000;
		handValue = texasHoldemGame.evaluateHand(hand, cardValue);
		Assert::AreNotEqual(static_cast<int>(HandValue::StraightFlush), static_cast<int>(handValue));
		Assert::AreEqual(expectedCardValue, cardValue);
	}

	TEST_METHOD(TestFourOfAKind)
	{
		// Create a game of TexasHoldem
		RandomAgent decisionAgent = RandomAgent();
		Deck deck = Deck();
		StraightIdentifier straightIdentifier = StraightIdentifier();
		TexasHoldem texasHoldemGame = TexasHoldem(1, 100.0f, decisionAgent, deck, straightIdentifier);

		// Set the shared cards
		std::array<Card, 5> sharedCards{ std::make_pair(Suit::Diamonds, Value::Five), std::make_pair(Suit::Diamonds, Value::Nine),
										 std::make_pair(Suit::Clubs, Value::Five), std::make_pair(Suit::Diamonds, Value::Six),
										 std::make_pair(Suit::Hearts, Value::Five) };
		texasHoldemGame.setSharedCards(sharedCards);

		// Create the hand of cards
		Hand hand = Hand();
		Card firstCard = std::make_pair(Suit::Spades, Value::Five);
		Card secondCard = std::make_pair(Suit::Spades, Value::Seven);
		hand.addCardToHand(firstCard, 0);
		hand.addCardToHand(secondCard, 1);

		// Check that we get the desired output
		uint64_t cardValue = 0;
		uint64_t expectedCardValue = 0b00000000010000000010000000;
		Assert::AreEqual(static_cast<int>(HandValue::FourOfAKind), static_cast<int>(texasHoldemGame.evaluateHand(hand, cardValue)));
		Assert::AreEqual(expectedCardValue, cardValue);

		// Change the hand to not have a four of a kind
		firstCard = std::make_pair(Suit::Hearts, Value::Nine);
		cardValue = 0;
		expectedCardValue = 0b00000000010000000010000000;
		hand.addCardToHand(firstCard, 0);
		Assert::AreNotEqual(static_cast<int>(HandValue::FourOfAKind), static_cast<int>(texasHoldemGame.evaluateHand(hand, cardValue)));
		Assert::AreEqual(expectedCardValue, cardValue);

		// Check the case when we have a non smallest value four of a kind
		cardValue = 0;
		expectedCardValue = 0b00000100000000000000010000;
		std::array<Card, 5> sharedCardsNew{ std::make_pair(Suit::Diamonds, Value::Five), std::make_pair(Suit::Diamonds, Value::Nine),
										 std::make_pair(Suit::Clubs, Value::Nine), std::make_pair(Suit::Diamonds, Value::Six),
										 std::make_pair(Suit::Hearts, Value::Five) };
		texasHoldemGame.setSharedCards(sharedCardsNew);
		firstCard = std::make_pair(Suit::Spades, Value::Nine);
		secondCard = std::make_pair(Suit::Hearts, Value::Nine);
		hand.addCardToHand(firstCard, 0);
		hand.addCardToHand(secondCard, 1);

		Assert::AreEqual(static_cast<int>(HandValue::FourOfAKind), static_cast<int>(texasHoldemGame.evaluateHand(hand, cardValue)));
		Assert::AreEqual(expectedCardValue, cardValue);
	}

	TEST_METHOD(TestFullHouse)
	{
		// Create a game of TexasHoldem
		RandomAgent decisionAgent = RandomAgent();
		Deck deck = Deck();
		StraightIdentifier straightIdentifier = StraightIdentifier();
		TexasHoldem texasHoldemGame = TexasHoldem(1, 100.0f, decisionAgent, deck, straightIdentifier);

		// Set the shared cards
		std::array<Card, 5> sharedCards{ std::make_pair(Suit::Diamonds, Value::Five), std::make_pair(Suit::Diamonds, Value::Nine),
										 std::make_pair(Suit::Clubs, Value::Five), std::make_pair(Suit::Diamonds, Value::Six),
										 std::make_pair(Suit::Hearts, Value::Five) };
		texasHoldemGame.setSharedCards(sharedCards);

		// Create the hand of cards
		Hand hand = Hand();
		Card firstCard = std::make_pair(Suit::Diamonds, Value::Nine);
		Card secondCard = std::make_pair(Suit::Spades, Value::Seven);
		hand.addCardToHand(firstCard, 0);
		hand.addCardToHand(secondCard, 1);

		// Check that we get the desired output
		uint64_t cardValue = 0;
		uint64_t expectedCardValue = 0b00000000010000000010000000;
		Assert::AreEqual(static_cast<int>(HandValue::FullHouse), static_cast<int>(texasHoldemGame.evaluateHand(hand, cardValue)));
		Assert::AreEqual(expectedCardValue, cardValue);

		// Change it so that there is no longer a Full House
		cardValue = 0;
		expectedCardValue = 0b0000010000000;
		firstCard = std::make_pair(Suit::Diamonds, Value::Eight);
		hand.addCardToHand(firstCard, 0);
		Assert::AreNotEqual(static_cast<int>(HandValue::FullHouse), static_cast<int>(texasHoldemGame.evaluateHand(hand, cardValue)));
		Assert::AreEqual(expectedCardValue, cardValue);

		// Add another Full House in a more complicated hand
		std::array<Card, 5> sharedCardsNew{ std::make_pair(Suit::Diamonds, Value::Five), std::make_pair(Suit::Diamonds, Value::Nine),
										 std::make_pair(Suit::Clubs, Value::Eight), std::make_pair(Suit::Hearts, Value::Five),
										 std::make_pair(Suit::Hearts, Value::Eight) };
		texasHoldemGame.setSharedCards(sharedCardsNew);

		// Change the cards held in the hand
		cardValue = 0;
		expectedCardValue = 0b00000010000000000000001000;
		firstCard = std::make_pair(Suit::Clubs, Value::Five);
		secondCard = std::make_pair(Suit::Hearts, Value::Eight);
		hand.addCardToHand(firstCard, 0);
		hand.addCardToHand(secondCard, 1);

		// Verify that the output is a full house
		Assert::AreEqual(static_cast<int>(HandValue::FullHouse), static_cast<int>(texasHoldemGame.evaluateHand(hand, cardValue)));
		Assert::AreEqual(expectedCardValue, cardValue);
	}

	TEST_METHOD(TestFlush)
	{
		// Create a game of TexasHoldem
		RandomAgent decisionAgent = RandomAgent();
		Deck deck = Deck();
		StraightIdentifier straightIdentifier = StraightIdentifier();
		TexasHoldem texasHoldemGame = TexasHoldem(1, 100.0f, decisionAgent, deck, straightIdentifier);

		// Set the shared cards
		std::array<Card, 5> sharedCards{ std::make_pair(Suit::Diamonds, Value::Five), std::make_pair(Suit::Diamonds, Value::Nine),
										 std::make_pair(Suit::Clubs, Value::Five), std::make_pair(Suit::Diamonds, Value::Six),
										 std::make_pair(Suit::Hearts, Value::Five) };
		texasHoldemGame.setSharedCards(sharedCards);

		// Create the hand of cards
		Hand hand = Hand();
		Card firstCard = std::make_pair(Suit::Diamonds, Value::Queen);
		Card secondCard = std::make_pair(Suit::Diamonds, Value::Ace);
		hand.addCardToHand(firstCard, 0);
		hand.addCardToHand(secondCard, 1);

		// Check that we get the desired output
		uint64_t cardValue = 0;
		uint64_t expectedCardValue = 0b1010010011000;
		Assert::AreEqual(static_cast<int>(HandValue::Flush), static_cast<int>(texasHoldemGame.evaluateHand(hand, cardValue)));
		Assert::AreEqual(expectedCardValue, cardValue);

		// Change it so that there is no longer a Full House
		firstCard = std::make_pair(Suit::Clubs, Value::Eight);
		hand.addCardToHand(firstCard, 0);
		cardValue = 0;
		expectedCardValue = 0b00000000010001000010000000;
		Assert::AreNotEqual(static_cast<int>(HandValue::Flush), static_cast<int>(texasHoldemGame.evaluateHand(hand, cardValue)));
		Assert::AreEqual(expectedCardValue, cardValue);

		// Add another Full House in a more complicated hand
		std::array<Card, 5> sharedCardsNew{ std::make_pair(Suit::Diamonds, Value::Ace), std::make_pair(Suit::Diamonds, Value::Queen),
										 std::make_pair(Suit::Diamonds, Value::Eight), std::make_pair(Suit::Hearts, Value::Five),
										 std::make_pair(Suit::Hearts, Value::Seven) };
		texasHoldemGame.setSharedCards(sharedCardsNew);

		// Change the cards held in the hand
		firstCard = std::make_pair(Suit::Diamonds, Value::Jack);
		secondCard = std::make_pair(Suit::Diamonds, Value::King);
		hand.addCardToHand(firstCard, 0);
		hand.addCardToHand(secondCard, 1);

		// Verify that the output is a full house
		cardValue = 0;
		expectedCardValue = 0b1111001000000;
		Assert::AreEqual(static_cast<int>(HandValue::Flush), static_cast<int>(texasHoldemGame.evaluateHand(hand, cardValue)));
		Assert::AreEqual(expectedCardValue, cardValue);
	}
	TEST_METHOD(TestStraight)
	{
		// Create a game of TexasHoldem
		RandomAgent decisionAgent = RandomAgent();
		Deck deck = Deck();
		StraightIdentifier straightIdentifier = StraightIdentifier();
		TexasHoldem texasHoldemGame = TexasHoldem(1, 100.0f, decisionAgent, deck, straightIdentifier);

		// Set the shared cards
		std::array<Card, 5> sharedCards{ std::make_pair(Suit::Diamonds, Value::Five), std::make_pair(Suit::Diamonds, Value::Nine),
										 std::make_pair(Suit::Clubs, Value::Six), std::make_pair(Suit::Clubs, Value::Five),
										 std::make_pair(Suit::Hearts, Value::Five) };
		texasHoldemGame.setSharedCards(sharedCards);

		// Create the hand of cards
		Hand hand = Hand();
		Card firstCard = std::make_pair(Suit::Diamonds, Value::Seven);
		Card secondCard = std::make_pair(Suit::Diamonds, Value::Eight);
		hand.addCardToHand(firstCard, 0);
		hand.addCardToHand(secondCard, 1);

		// Check that we get the desired output
		uint64_t cardValue = 0;
		uint64_t expectedCardValue = 0b0000010000000;
		Assert::AreEqual(static_cast<int>(HandValue::Straight), static_cast<int>(texasHoldemGame.evaluateHand(hand, cardValue)));
		Assert::AreEqual(expectedCardValue, cardValue);
		
		// Change it so that there is no longer a Full House
		cardValue = 0;
		expectedCardValue = 0b00000000010001000010000000;
		firstCard = std::make_pair(Suit::Clubs, Value::Ace);
		hand.addCardToHand(firstCard, 0);
		Assert::AreNotEqual(static_cast<int>(HandValue::Flush), static_cast<int>(texasHoldemGame.evaluateHand(hand, cardValue)));

		// Add another Full House in a more complicated hand
		std::array<Card, 5> sharedCardsNew{ std::make_pair(Suit::Diamonds, Value::King), std::make_pair(Suit::Diamonds, Value::Queen),
										 std::make_pair(Suit::Clubs, Value::Ace), std::make_pair(Suit::Spades, Value::Five),
										 std::make_pair(Suit::Hearts, Value::Seven) };
		texasHoldemGame.setSharedCards(sharedCardsNew);

		// Change the cards held in the hand
		firstCard = std::make_pair(Suit::Spades, Value::Jack);
		secondCard = std::make_pair(Suit::Spades, Value::Ten);
		hand.addCardToHand(firstCard, 0);
		hand.addCardToHand(secondCard, 1);

		// Verify that the output is a full house
		cardValue = 0;
		expectedCardValue = 0b1000000000000;
		Assert::AreEqual(static_cast<int>(HandValue::Straight), static_cast<int>(texasHoldemGame.evaluateHand(hand, cardValue)));
		Assert::AreEqual(expectedCardValue, cardValue);
	}

	TEST_METHOD(TestThreeOfAKind)
	{
		// Create a game of TexasHoldem
		RandomAgent decisionAgent = RandomAgent();
		Deck deck = Deck();
		StraightIdentifier straightIdentifier = StraightIdentifier();
		TexasHoldem texasHoldemGame = TexasHoldem(1, 100.0f, decisionAgent, deck, straightIdentifier);

		// Set the shared cards
		std::array<Card, 5> sharedCards{ std::make_pair(Suit::Diamonds, Value::Ace), std::make_pair(Suit::Diamonds, Value::Five),
										 std::make_pair(Suit::Clubs, Value::Six), std::make_pair(Suit::Clubs, Value::Nine),
										 std::make_pair(Suit::Hearts, Value::Nine) };
		texasHoldemGame.setSharedCards(sharedCards);

		// Create the hand of cards
		Hand hand = Hand();
		Card firstCard = std::make_pair(Suit::Diamonds, Value::Nine);
		Card secondCard = std::make_pair(Suit::Diamonds, Value::King);
		hand.addCardToHand(firstCard, 0);
		hand.addCardToHand(secondCard, 1);

		// Check that we get the desired output
		uint64_t cardValue = 0;
		uint64_t expectedCardValue = 0b00000100000001100000000000;
		Assert::AreEqual(static_cast<int>(HandValue::ThreeOfAKind), static_cast<int>(texasHoldemGame.evaluateHand(hand, cardValue)));
		Assert::AreEqual(expectedCardValue, cardValue);

		// Change it so that there is no longer a Full House
		firstCard = std::make_pair(Suit::Clubs, Value::Ace);
		hand.addCardToHand(firstCard, 0);
		cardValue = 0;
		expectedCardValue = 0b10000100000000100000000000;
		Assert::AreNotEqual(static_cast<int>(HandValue::ThreeOfAKind), static_cast<int>(texasHoldemGame.evaluateHand(hand, cardValue)));
		Assert::AreEqual(expectedCardValue, cardValue);

		// Add another Full House in a more complicated hand
		std::array<Card, 5> sharedCardsNew{ std::make_pair(Suit::Diamonds, Value::King), std::make_pair(Suit::Diamonds, Value::Queen),
										 std::make_pair(Suit::Clubs, Value::Ace), std::make_pair(Suit::Spades, Value::Five),
										 std::make_pair(Suit::Hearts, Value::Seven) };
		texasHoldemGame.setSharedCards(sharedCardsNew);

		// Change the cards held in the hand
		firstCard = std::make_pair(Suit::Spades, Value::King);
		secondCard = std::make_pair(Suit::Clubs, Value::King);
		hand.addCardToHand(firstCard, 0);
		hand.addCardToHand(secondCard, 1);

		// Verify that the output is a full house
		cardValue = 0;
		expectedCardValue = 0b01000000000001010000000000;
		Assert::AreEqual(static_cast<int>(HandValue::ThreeOfAKind), static_cast<int>(texasHoldemGame.evaluateHand(hand, cardValue)));
		Assert::AreEqual(expectedCardValue, cardValue);
	}

	TEST_METHOD(TestTwoPair)
	{
		// Create a game of TexasHoldem
		RandomAgent decisionAgent = RandomAgent();
		Deck deck = Deck();
		StraightIdentifier straightIdentifier = StraightIdentifier();
		TexasHoldem texasHoldemGame = TexasHoldem(1, 100.0f, decisionAgent, deck, straightIdentifier);

		// Set the shared cards
		std::array<Card, 5> sharedCards{ std::make_pair(Suit::Diamonds, Value::Ace), std::make_pair(Suit::Diamonds, Value::Five),
										 std::make_pair(Suit::Clubs, Value::Six), std::make_pair(Suit::Clubs, Value::Nine),
										 std::make_pair(Suit::Hearts, Value::Nine) };
		texasHoldemGame.setSharedCards(sharedCards);

		// Create the hand of cards
		Hand hand = Hand();
		Card firstCard = std::make_pair(Suit::Diamonds, Value::Six);
		Card secondCard = std::make_pair(Suit::Diamonds, Value::King);
		hand.addCardToHand(firstCard, 0);
		hand.addCardToHand(secondCard, 1);

		// Check that we get the desired output
		uint64_t cardValue = 0;
		uint64_t expectedCardValue = 0b00000100100001000000000000;
		Assert::AreEqual(static_cast<int>(HandValue::TwoPair), static_cast<int>(texasHoldemGame.evaluateHand(hand, cardValue)));
		Assert::AreEqual(expectedCardValue, cardValue);

		// Change it so that there is no longer a Full House
		cardValue = 0;
		expectedCardValue = 0b00000100000001100000010000;
		firstCard = std::make_pair(Suit::Clubs, Value::Two);
		hand.addCardToHand(firstCard, 0);
		Assert::AreNotEqual(static_cast<int>(HandValue::TwoPair), static_cast<int>(texasHoldemGame.evaluateHand(hand, cardValue)));
		Assert::AreEqual(expectedCardValue, cardValue);

		// Add another Full House in a more complicated hand
		std::array<Card, 5> sharedCardsNew{ std::make_pair(Suit::Diamonds, Value::King), std::make_pair(Suit::Clubs, Value::King),
										 std::make_pair(Suit::Clubs, Value::Ace), std::make_pair(Suit::Spades, Value::Five),
										 std::make_pair(Suit::Hearts, Value::Seven) };
		texasHoldemGame.setSharedCards(sharedCardsNew);

		// Change the cards held in the hand
		cardValue = 0;
		expectedCardValue = 0b01000100000001000000000000;
		firstCard = std::make_pair(Suit::Spades, Value::Nine);
		secondCard = std::make_pair(Suit::Clubs, Value::Nine);
		hand.addCardToHand(firstCard, 0);
		hand.addCardToHand(secondCard, 1);

		// Verify that the output is a full house
		Assert::AreEqual(static_cast<int>(HandValue::TwoPair), static_cast<int>(texasHoldemGame.evaluateHand(hand, cardValue)));
		Assert::AreEqual(expectedCardValue, cardValue);
	}

	TEST_METHOD(TestPair)
	{
		// Create a game of TexasHoldem
		RandomAgent decisionAgent = RandomAgent();
		Deck deck = Deck();
		StraightIdentifier straightIdentifier = StraightIdentifier();
		TexasHoldem texasHoldemGame = TexasHoldem(1, 100.0f, decisionAgent, deck, straightIdentifier);

		// Set the shared cards
		std::array<Card, 5> sharedCards{ std::make_pair(Suit::Diamonds, Value::Ace), std::make_pair(Suit::Diamonds, Value::Five),
										 std::make_pair(Suit::Clubs, Value::Six), std::make_pair(Suit::Clubs, Value::Nine),
										 std::make_pair(Suit::Hearts, Value::Jack) };
		texasHoldemGame.setSharedCards(sharedCards);

		// Create the hand of cards
		Hand hand = Hand();
		Card firstCard = std::make_pair(Suit::Diamonds, Value::Six);
		Card secondCard = std::make_pair(Suit::Diamonds, Value::King);
		hand.addCardToHand(firstCard, 0);
		hand.addCardToHand(secondCard, 1);

		// Check that we get the desired output
		uint64_t cardValue = 0;
		uint64_t expectedCardValue = 0b00000000100001101000000000;
		Assert::AreEqual(static_cast<int>(HandValue::Pair), static_cast<int>(texasHoldemGame.evaluateHand(hand, cardValue)));
		Assert::AreEqual(expectedCardValue, cardValue);

		// Change it so that there is no longer a Full House
		firstCard = std::make_pair(Suit::Clubs, Value::Two);
		hand.addCardToHand(firstCard, 0);
		cardValue = 0;
		expectedCardValue = 0b1101010010000;
		Assert::AreNotEqual(static_cast<int>(HandValue::Pair), static_cast<int>(texasHoldemGame.evaluateHand(hand, cardValue)));
		Assert::AreEqual(expectedCardValue, cardValue);

		// Add another Full House in a more complicated hand
		std::array<Card, 5> sharedCardsNew{ std::make_pair(Suit::Diamonds, Value::King), std::make_pair(Suit::Clubs, Value::Queen),
										 std::make_pair(Suit::Clubs, Value::Ace), std::make_pair(Suit::Spades, Value::Five),
										 std::make_pair(Suit::Hearts, Value::Seven) };
		texasHoldemGame.setSharedCards(sharedCardsNew);

		// Change the cards held in the hand
		firstCard = std::make_pair(Suit::Spades, Value::Nine);
		secondCard = std::make_pair(Suit::Clubs, Value::Nine);
		hand.addCardToHand(firstCard, 0);
		hand.addCardToHand(secondCard, 1);

		// Verify that the output is a full house
		cardValue = 0;
		expectedCardValue = 0b00000100000001110000000000;
		Assert::AreEqual(static_cast<int>(HandValue::Pair), static_cast<int>(texasHoldemGame.evaluateHand(hand, cardValue)));
		Assert::AreEqual(expectedCardValue, cardValue);
	}

    TEST_METHOD(TestHighCard)
	{
		// Create a game of TexasHoldem
		RandomAgent decisionAgent = RandomAgent();
		Deck deck = Deck();
		StraightIdentifier straightIdentifier = StraightIdentifier();
		TexasHoldem texasHoldemGame = TexasHoldem(1, 100.0f, decisionAgent, deck, straightIdentifier);

		// Set the shared cards
		std::array<Card, 5> sharedCards{ std::make_pair(Suit::Diamonds, Value::Ace), std::make_pair(Suit::Diamonds, Value::Five),
										 std::make_pair(Suit::Clubs, Value::Six), std::make_pair(Suit::Clubs, Value::Nine),
										 std::make_pair(Suit::Hearts, Value::Jack) };
		texasHoldemGame.setSharedCards(sharedCards);

		// Create the hand of cards
		Hand hand = Hand();
		Card firstCard = std::make_pair(Suit::Diamonds, Value::Eight);
		Card secondCard = std::make_pair(Suit::Diamonds, Value::King);
		hand.addCardToHand(firstCard, 0);
		hand.addCardToHand(secondCard, 1);

		// Check that we get the desired output
		uint64_t cardValue = 0;
		uint64_t expectedCardValue = 0b1101011000000;
		Assert::AreEqual(static_cast<int>(HandValue::HighCard), static_cast<int>(texasHoldemGame.evaluateHand(hand, cardValue)));
		Assert::AreEqual(expectedCardValue, cardValue);

		// Change it so that there is no longer a Full House
		secondCard = std::make_pair(Suit::Clubs, Value::Seven);
		hand.addCardToHand(secondCard, 1);
		cardValue = 0;
		expectedCardValue = 0b0000010000000;
		Assert::AreNotEqual(static_cast<int>(HandValue::HighCard), static_cast<int>(texasHoldemGame.evaluateHand(hand, cardValue)));
		Assert::AreEqual(expectedCardValue, cardValue);

		// Add another Full House in a more complicated hand
		std::array<Card, 5> sharedCardsNew{ std::make_pair(Suit::Diamonds, Value::King), std::make_pair(Suit::Clubs, Value::Queen),
										 std::make_pair(Suit::Clubs, Value::Ace), std::make_pair(Suit::Spades, Value::Eight),
										 std::make_pair(Suit::Hearts, Value::Seven) };
		texasHoldemGame.setSharedCards(sharedCardsNew);

		// Change the cards held in the hand
		firstCard = std::make_pair(Suit::Spades, Value::Jack);
		secondCard = std::make_pair(Suit::Clubs, Value::Nine);
		hand.addCardToHand(firstCard, 0);
		hand.addCardToHand(secondCard, 1);

		// Verify that the output is a full house
		cardValue = 0;
		expectedCardValue = 0b1111010000000;
		Assert::AreEqual(static_cast<int>(HandValue::HighCard), static_cast<int>(texasHoldemGame.evaluateHand(hand, cardValue)));
		Assert::AreEqual(expectedCardValue, cardValue);
	}
};


TEST_CLASS(TestAgent)
{
public:
	TEST_METHOD(TestDecision)
	{
		int stateSize = 2;
		LinearAgent agent = LinearAgent(stateSize);

		// State of all 0s should return a decision of Call
		std::vector<float> state;
		for (int i = 0; i < stateSize; i++)
		{
			state.push_back(0.0f);
		}
		float maxRaise = 10.0f;
		float minRaise = 1.0f;
		Decision decision = agent.makeDecision(state, minRaise, maxRaise);

		Assert::AreEqual(static_cast<int>(decision.play), static_cast<int>(Play::Fold));
		Assert::AreEqual(static_cast<int>(decision.betAmount), 0);
	}

};



TEST_CLASS(TestGeneticAlgorithm)
{
public:
	TEST_METHOD(TestEvaluation)
	{
		int iniNumIndividuals = 2;
		int numOpponents = 2;
		int numGamesPerPair = 2;
		GeneticAlgorithm ga = GeneticAlgorithm(iniNumIndividuals, numOpponents, numGamesPerPair);
		ga.evaluate();

		// Assert all individuals competed 'numOpponents' times
		for (int i=0; i<ga.getNumIndividuals(); i++)
		{
			Assert::AreEqual(ga.getIndividualByIndex(i).getNumPlayedCompetitions(), numOpponents);
		}

	}

	TEST_METHOD(TestCrossOver)
	{
		int iniNumIndividuals = 3;
		int numOpponents = 2;
		int numGamesPerPair = 2;
		GeneticAlgorithm ga = GeneticAlgorithm(iniNumIndividuals, numOpponents, numGamesPerPair);
		std::vector<float> strategy0 = ga.getIndividualStrategyByIndex(0);
		std::vector<float> strategy1 = ga.getIndividualStrategyByIndex(1);
		std::vector<float> strategy2 = ga.getIndividualStrategyByIndex(2);

		int lastIdx = 5;
		float s0 = strategy0[lastIdx];
		float s1 = strategy1[lastIdx];
		float s2 = strategy2[lastIdx];

		ga.crossOver();
		std::vector<float> crossedStrategy0 = ga.getIndividualStrategyByIndex(0);
		std::vector<float> crossedStrategy1 = ga.getIndividualStrategyByIndex(1);
		std::vector<float> crossedStrategy2 = ga.getIndividualStrategyByIndex(2);

		// We don't know in which index was the cross over, but at least the last element of the array 
		// should be changed in (thetaSize-1)/thetaSize of the times.
		Assert::AreNotEqual(s0, crossedStrategy0[lastIdx]);
		Assert::AreNotEqual(s1, crossedStrategy1[lastIdx]);

		// 3rd individual's strategy is not crossed because the number of individuals is odd
		Assert::AreEqual(s2, crossedStrategy2[lastIdx]);
	}

	TEST_METHOD(TestMutate)
	{
		int iniNumIndividuals = 3;
		int numOpponents = 2;
		int numGamesPerPair = 2;
		GeneticAlgorithm ga = GeneticAlgorithm(iniNumIndividuals, numOpponents, numGamesPerPair);
		std::vector<float> strategy0 = ga.getIndividualStrategyByIndex(0);
		std::vector<float> strategy1 = ga.getIndividualStrategyByIndex(1);
		std::vector<float> strategy2 = ga.getIndividualStrategyByIndex(2);

		float s0 = strategy0[0];
		float s1 = strategy1[0];
		float s2 = strategy2[0];

		// Mutate all values
		ga.mutate(1.0);
		std::vector<float> mutatedStrategy0 = ga.getIndividualStrategyByIndex(0);
		std::vector<float> mutatedStrategy1 = ga.getIndividualStrategyByIndex(1);
		std::vector<float> mutatedStrategy2 = ga.getIndividualStrategyByIndex(2);

		Assert::AreNotEqual(s0, mutatedStrategy0[0]);
		Assert::AreNotEqual(s1, mutatedStrategy1[0]);
		Assert::AreNotEqual(s2, mutatedStrategy2[0]);

		float ms0 = mutatedStrategy0[0];
		float ms1 = mutatedStrategy1[0];
		float ms2 = mutatedStrategy2[0];

		// Not mutate any value
		ga.mutate(0.0);
		std::vector<float> mutatedAgainStrategy0 = ga.getIndividualStrategyByIndex(0);
		std::vector<float> mutatedAgainStrategy1 = ga.getIndividualStrategyByIndex(1);
		std::vector<float> mutatedAgainStrategy2 = ga.getIndividualStrategyByIndex(2);

		Assert::AreEqual(ms0, mutatedAgainStrategy0[0]);
		Assert::AreEqual(ms1, mutatedAgainStrategy1[0]);
		Assert::AreEqual(ms2, mutatedAgainStrategy2[0]);
	}

};
