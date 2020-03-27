#pragma once
#include "Robot.h"
#include "Ball.h"
#include <list>

typedef list<Robot*> listRobot;
typedef list<Robot*>::iterator itRobot;

class Mayor
{
public:
	Mayor();
	~Mayor();
	uint loadPopulation(); // reads the population from files
	uint createPopulation(unique_ptr<City>& pCity, uint aPopulation, uint aCrossing);
	void randomPosition(unique_ptr<City>& pCity, uint aMin, uint aMax, uint& PosX, uint& PosY);

private:
	uint createPopulation(uint aPopulation);
	void resetPopulation(unique_ptr<City>& pCity);

	listRobot m_robots;
};

