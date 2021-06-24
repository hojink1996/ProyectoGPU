#include <array>
#include "pch.h"
#include "CppUnitTest.h"
#include "../ProyectoGPU/deck.h"
#include "../ProyectoGPU/deck.cpp"
#include "../ProyectoGPU/card.h"
#include "../ProyectoGPU/hand.h"
#include "../ProyectoGPU/hand.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(TestDeck)
{
public:
	TEST_METHOD(TestCreation)
	{
		// Make a band new card and draw the first card (should be the ace of clubs)
		Deck testDeck = Deck();
		std::pair<Suit, Value> firstCard = testDeck.drawCard();
		std::pair<Suit, Value> expectedCard = std::make_pair(Suit::Clubs, Value::Ace);

		// Verify that we got the expected card
		Assert::AreEqual(static_cast<int>(expectedCard.first), static_cast<int>(firstCard.first));
		Assert::AreEqual(static_cast<int>(expectedCard.second), static_cast<int>(firstCard.second));
	}

	TEST_METHOD(TestDrawingCards)
	{
		// Make a brand new deck of cards and draw two cards
		Deck testDeck = Deck();
		std::pair<Suit, Value> firstCard = testDeck.drawCard();
		std::pair<Suit, Value> secondCard = testDeck.drawCard();

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
		std::pair<Suit, Value> firstCard = testDeck.drawCard();

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
		std::array<std::pair<Suit, Value>, 2> initialCards = hand.getHand();
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
		std::pair<Suit, Value> firstCard = deck.drawCard();
		std::pair<Suit, Value> secondCard = deck.drawCard();

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