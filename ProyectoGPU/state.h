#pragma once

/*
The state of a player is given by a set of parameters that we define. Currently, we consider 22 parameters
for the state vector.
*/
struct State
{
	float values[21];
};