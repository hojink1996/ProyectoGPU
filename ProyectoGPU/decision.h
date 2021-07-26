#pragma once

/*
Represents the possible plays that a player can do.
*/
enum class Play
{
	Fold,
	Call,
	Raise,
	Invalid
};

/*
Used to represent each decision that the user can do (which is given by
the play they are doing and the amount they are betting).
*/
struct Decision
{
	Play play;
	int betAmount;
};
