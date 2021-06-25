#include "card.h"

bool CardEvaluation::orderCardValue(Value left, Value right)
{
	return static_cast<int>(left) < static_cast<int>(right);
}