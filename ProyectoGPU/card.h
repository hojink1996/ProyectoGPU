#pragma once
#include <vector>

/*
Used to represent the different possible suits of a card.
*/
enum class Suit
{
	Clubs,
	Diamonds,
	Hearts,
	Spades,
	Invalid
};

/*
Used to represent the different possible values of a card.
*/
enum class Value
{
	Ace,
	Two,
	Three,
	Four,
	Five,
	Six,
	Seven,
	Eight,
	Nine,
	Ten,
	Jack,
	Queen,
	King,
	Invalid
};

/*
Function used to order the card values when sorting
*/
namespace CardEvaluation {
	bool orderCardValue(Value left, Value right);
};

typedef std::pair<Suit, Value> Card;
constexpr Card invalidCard{ std::make_pair(Suit::Invalid, Value::Invalid) };
