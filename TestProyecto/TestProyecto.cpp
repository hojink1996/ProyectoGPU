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
#include "../ProyectoGPU/straight-identifier.h"
#include "../ProyectoGPU/straight-identifier.cpp"
#include "../ProyectoGPU/texas-holdem.h"
#include "../ProyectoGPU/texas-holdem.cpp"
#include "../ProyectoGPU/agent.h"
#include "../ProyectoGPU/agent.cpp"
#include "../ProyectoGPU/individual.h"
#include "../ProyectoGPU/individual.cpp"
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
		Assert::IsTrue(straightIdentifier.hasRoyalStraight(basicRoyalStraight));
		Assert::IsTrue(straightIdentifier.hasRoyalStraight(royalStraightWithExtra));
		Assert::IsFalse(straightIdentifier.hasRoyalStraight(notRoyalStraight));
		Assert::IsFalse(straightIdentifier.hasRoyalStraight(isStraightButNotRoyalStraight));
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
		Assert::IsTrue(straightIdentifier.hasStraightFlush(basicStraight));
		Assert::IsTrue(straightIdentifier.hasStraightFlush(simpleMixedStraight));
		Assert::IsTrue(straightIdentifier.hasStraightFlush(mixedStraight));
		Assert::IsFalse(straightIdentifier.hasStraightFlush(notStraight));
		Assert::IsFalse(straightIdentifier.hasStraightFlush(notStraightThrees));
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
		HandValue handValue =  texasHoldemGame.evaluateHand(hand);

		// Check that we get the expected value
		Assert::AreEqual(static_cast<int>(HandValue::RoyalFlush), static_cast<int>(handValue));

		// Change to the scenario that is not a royal flus
		texasHoldemGame.setSharedCards(sharedCardNotRoyal);
		handValue = texasHoldemGame.evaluateHand(hand);
		Assert::AreEqual(static_cast<int>(HandValue::Flush), static_cast<int>(handValue));
	}
	TEST_METHOD(TestStraghtFlush)
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
		HandValue handValue = texasHoldemGame.evaluateHand(hand);
		Assert::AreEqual(static_cast<int>(HandValue::StraightFlush), static_cast<int>(handValue));
		hand.resetHand();

		// Check the case for an incorrect hand
		firstCard = std::make_pair(Suit::Clubs, Value::Seven);
		secondCard = std::make_pair(Suit::Clubs, Value::Eight);

		// Check that we don't get a straight flush in this case
		handValue = texasHoldemGame.evaluateHand(hand);
		Assert::AreNotEqual(static_cast<int>(HandValue::StraightFlush), static_cast<int>(handValue));
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
		Assert::AreEqual(static_cast<int>(HandValue::FourOfAKind), static_cast<int>(texasHoldemGame.evaluateHand(hand)));

		// Change the hand to not have a four of a kind
		firstCard = std::make_pair(Suit::Hearts, Value::Nine);
		hand.addCardToHand(firstCard, 0);
		Assert::AreNotEqual(static_cast<int>(HandValue::FourOfAKind), static_cast<int>(texasHoldemGame.evaluateHand(hand)));

		// Check the case when we have a non smallest value four of a kind
		std::array<Card, 5> sharedCardsNew{ std::make_pair(Suit::Diamonds, Value::Five), std::make_pair(Suit::Diamonds, Value::Nine),
										 std::make_pair(Suit::Clubs, Value::Nine), std::make_pair(Suit::Diamonds, Value::Six),
										 std::make_pair(Suit::Hearts, Value::Five) };
		texasHoldemGame.setSharedCards(sharedCardsNew);
		firstCard = std::make_pair(Suit::Spades, Value::Nine);
		secondCard = std::make_pair(Suit::Hearts, Value::Nine);
		hand.addCardToHand(firstCard, 0);
		hand.addCardToHand(secondCard, 1);

		Assert::AreEqual(static_cast<int>(HandValue::FourOfAKind), static_cast<int>(texasHoldemGame.evaluateHand(hand)));
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
		Assert::AreEqual(static_cast<int>(HandValue::FullHouse), static_cast<int>(texasHoldemGame.evaluateHand(hand)));

		// Change it so that there is no longer a Full House
		firstCard = std::make_pair(Suit::Diamonds, Value::Eight);
		hand.addCardToHand(firstCard, 0);
		Assert::AreNotEqual(static_cast<int>(HandValue::FullHouse), static_cast<int>(texasHoldemGame.evaluateHand(hand)));

		// Add another Full House in a more complicated hand
		std::array<Card, 5> sharedCardsNew{ std::make_pair(Suit::Diamonds, Value::Five), std::make_pair(Suit::Diamonds, Value::Nine),
										 std::make_pair(Suit::Clubs, Value::Eight), std::make_pair(Suit::Hearts, Value::Five),
										 std::make_pair(Suit::Hearts, Value::Eight) };
		texasHoldemGame.setSharedCards(sharedCardsNew);

		// Change the cards held in the hand
		firstCard = std::make_pair(Suit::Clubs, Value::Five);
		secondCard = std::make_pair(Suit::Hearts, Value::Eight);
		hand.addCardToHand(firstCard, 0);
		hand.addCardToHand(secondCard, 1);

		// Verify that the output is a full house
		Assert::AreEqual(static_cast<int>(HandValue::FullHouse), static_cast<int>(texasHoldemGame.evaluateHand(hand)));
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
		Assert::AreEqual(static_cast<int>(HandValue::Flush), static_cast<int>(texasHoldemGame.evaluateHand(hand)));

		// Change it so that there is no longer a Full House
		firstCard = std::make_pair(Suit::Clubs, Value::Eight);
		hand.addCardToHand(firstCard, 0);
		Assert::AreNotEqual(static_cast<int>(HandValue::Flush), static_cast<int>(texasHoldemGame.evaluateHand(hand)));

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
		Assert::AreEqual(static_cast<int>(HandValue::Flush), static_cast<int>(texasHoldemGame.evaluateHand(hand)));
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
		Assert::AreEqual(static_cast<int>(HandValue::Straight), static_cast<int>(texasHoldemGame.evaluateHand(hand)));

		// Change it so that there is no longer a Full House
		firstCard = std::make_pair(Suit::Clubs, Value::Ace);
		hand.addCardToHand(firstCard, 0);
		Assert::AreNotEqual(static_cast<int>(HandValue::Flush), static_cast<int>(texasHoldemGame.evaluateHand(hand)));

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
		Assert::AreEqual(static_cast<int>(HandValue::Straight), static_cast<int>(texasHoldemGame.evaluateHand(hand)));
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
		Assert::AreEqual(static_cast<int>(HandValue::ThreeOfAKind), static_cast<int>(texasHoldemGame.evaluateHand(hand)));

		// Change it so that there is no longer a Full House
		firstCard = std::make_pair(Suit::Clubs, Value::Ace);
		hand.addCardToHand(firstCard, 0);
		Assert::AreNotEqual(static_cast<int>(HandValue::ThreeOfAKind), static_cast<int>(texasHoldemGame.evaluateHand(hand)));

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
		Assert::AreEqual(static_cast<int>(HandValue::ThreeOfAKind), static_cast<int>(texasHoldemGame.evaluateHand(hand)));
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
		Assert::AreEqual(static_cast<int>(HandValue::TwoPair), static_cast<int>(texasHoldemGame.evaluateHand(hand)));

		// Change it so that there is no longer a Full House
		firstCard = std::make_pair(Suit::Clubs, Value::Two);
		hand.addCardToHand(firstCard, 0);
		Assert::AreNotEqual(static_cast<int>(HandValue::TwoPair), static_cast<int>(texasHoldemGame.evaluateHand(hand)));

		// Add another Full House in a more complicated hand
		std::array<Card, 5> sharedCardsNew{ std::make_pair(Suit::Diamonds, Value::King), std::make_pair(Suit::Clubs, Value::King),
										 std::make_pair(Suit::Clubs, Value::Ace), std::make_pair(Suit::Spades, Value::Five),
										 std::make_pair(Suit::Hearts, Value::Seven) };
		texasHoldemGame.setSharedCards(sharedCardsNew);

		// Change the cards held in the hand
		firstCard = std::make_pair(Suit::Spades, Value::Nine);
		secondCard = std::make_pair(Suit::Clubs, Value::Nine);
		hand.addCardToHand(firstCard, 0);
		hand.addCardToHand(secondCard, 1);

		// Verify that the output is a full house
		Assert::AreEqual(static_cast<int>(HandValue::TwoPair), static_cast<int>(texasHoldemGame.evaluateHand(hand)));
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
		Assert::AreEqual(static_cast<int>(HandValue::Pair), static_cast<int>(texasHoldemGame.evaluateHand(hand)));

		// Change it so that there is no longer a Full House
		firstCard = std::make_pair(Suit::Clubs, Value::Two);
		hand.addCardToHand(firstCard, 0);
		Assert::AreNotEqual(static_cast<int>(HandValue::Pair), static_cast<int>(texasHoldemGame.evaluateHand(hand)));

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
		Assert::AreEqual(static_cast<int>(HandValue::Pair), static_cast<int>(texasHoldemGame.evaluateHand(hand)));
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
		Assert::AreEqual(static_cast<int>(HandValue::HighCard), static_cast<int>(texasHoldemGame.evaluateHand(hand)));

		// Change it so that there is no longer a Full House
		secondCard = std::make_pair(Suit::Clubs, Value::Seven);
		hand.addCardToHand(secondCard, 1);
		Assert::AreNotEqual(static_cast<int>(HandValue::HighCard), static_cast<int>(texasHoldemGame.evaluateHand(hand)));

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
		Assert::AreEqual(static_cast<int>(HandValue::HighCard), static_cast<int>(texasHoldemGame.evaluateHand(hand)));
	}
};


TEST_CLASS(TestAgent)
{
public:
	TEST_METHOD(TestDecision)
	{
		LinearAgent agent;
		agent.initialize_theta();
		
		// State of all 0s should return a decision of Call
		float state[10] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
		float maxBet = 10.0f;
		float minBet = 1.0f;
		Decision decision = agent.makeDecision(state, maxBet, minBet);

		Assert::AreEqual(static_cast<int>(decision.play), static_cast<int>(Play::Call));
		Assert::AreEqual(static_cast<int>(decision.betAmount), static_cast<int>(minBet));
	}

};



TEST_CLASS(TestGeneticAlgorithm)
{
public:
	TEST_METHOD(TestEvaluation)
	{
		int iniNumIndividuals = 4;
		int numOpponents = 2;
		GeneticAlgorithm ga = GeneticAlgorithm(iniNumIndividuals, numOpponents);
		ga.evaluate();

		int totalWin = 0;

		// Assert all individuals competed 'numOpponents' times
		for (int i=0; i<ga.getNumIndividuals(); i++)
		{
			Assert::AreEqual(ga.currentIndividuals[i].getNumPlayedGames(), numOpponents);
			totalWin += ga.currentIndividuals[i].getNumWins();
		}

		// Assert total number of wins is equal to total number of competitions
		Assert::AreEqual(totalWin, iniNumIndividuals * numOpponents / 2);
	}

};
