#pragma once
#include "Robot.h"
#include <list>

typedef list<Robot*> listRobot;
typedef list<Robot*>::iterator itRobot;

class Mayor
{
public:
	Mayor();
	uint loadPopulation(); // reads the population from files
	uint createPopulation(uint aPopulation);
	uint testPopulation(City& aCity, uint aPopulation, uint aGenerations, uint aCrossing);

private:
	listRobot m_robots;
};

