#pragma once
#include "genetic-algorithm.h"
#include "texas-holdem.h"

class Context
{
private:
	GeneticAlgorithm& geneticAlgorithm;
public:
	Context(GeneticAlgorithm* ga);
	void run();
};