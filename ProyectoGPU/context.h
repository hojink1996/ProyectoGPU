#pragma once
#include "genetic-algorithm.h"
#include "texas-holdem.h"

class Context
{
private:
	bool parallelize = true;
public:
	Context(bool parallelize);
	void run();
};