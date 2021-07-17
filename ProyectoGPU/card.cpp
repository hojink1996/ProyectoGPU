#include "card.h"

bool CardEvaluation::orderCardValue(Value left, Value right)
{
	return static_cast<int>(left) < static_cast<int>(right);
}

bool CardEvaluation::leftCardHasGreaterValue(Value left, Value right)
{
	int leftValue = (static_cast<int>(left) + 12) % 13;
	int rightValue = (static_cast<int>(right) + 12) % 13;
	leftValue = (left == Value::Invalid) ? -1 : leftValue;
	rightValue = (right == Value::Invalid) ? -1 : rightValue;
	return leftValue > rightValue;
}