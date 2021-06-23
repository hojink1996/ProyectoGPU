#include "pch.h"
#include "CppUnitTest.h"
#include "../ProyectoGPU/deck.h"
#include "../ProyectoGPU/deck.cpp"
#include "../ProyectoGPU/card.h"

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
};
