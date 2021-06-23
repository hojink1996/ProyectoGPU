#pragma once

enum class Play
{
	Fold,
	Call,
	Raise,
	Invalid
};

struct Decision
{
	Play play;
	float betAmount;
};
