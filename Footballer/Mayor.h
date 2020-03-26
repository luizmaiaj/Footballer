#pragma once
#include "Robot.h"
#include <list>

typedef list<Robot*> listRobot;
typedef list<Robot*>::iterator itRobot;

class Mayor
{
public:
	Mayor(uint aPopulation);
	uint loadPopulation(); // reads the population from files
	uint createPopulation();
	uint testPopulation(uint aPopulation, uint aGenerations, uint aCrossing);

private:
	listRobot m_robots;
};

