#include "Mayor.h"

Mayor::Mayor()
{
}

Mayor::~Mayor()
{
	for (itRobot it = m_robots.begin(); it != m_robots.end(); it++)
	{
		Robot* pR = *it;

		if (pR) delete pR;
	}
}

uint Mayor::loadPopulation()
{
	return 0; // to be implemented when we have sample files
}

uint Mayor::createPopulation(uint aPopulation)
{
	if (m_robots.size() >= aPopulation) return aPopulation;

	for (uint i = m_robots.size(); i < aPopulation; i++)
	{
		Robot* pR = new Robot(); // new robot

		pR->birth(); // allocate the random tree

		m_robots.push_back(pR); // store
	}

	return aPopulation;
}

void Mayor::resetPopulation(unique_ptr<City>& pCity)
{
	for (itRobot it = m_robots.begin(); it != m_robots.end(); it++)
	{
		Robot* pR = *it;
		uint PosX{ 0 }, PosY{ 0 };

		randomPosition(pCity, 1, HEIGHT - 1, PosX, PosY);

		pR->reset(PosX, PosY, ANGLE * (rand() % (360 / ANGLE)));
	}
}

void Mayor::randomPosition(unique_ptr<City>& pCity, uint aMin, uint aMax, uint& PosX, uint& PosY)
{
	do
	{
		aMax -= aMin;

		PosX = (rand() % aMax) + aMin;

		PosY = (rand() % aMax) + aMin;
	}
	while (pCity->collidesEnvironment(PosX, PosY));
}

uint Mayor::createPopulation(unique_ptr<City>& pCity, uint aPopulation, uint aCrossing)
{
	aPopulation = createPopulation(aPopulation);	// creates or completes a population with new individuals
	resetPopulation(pCity);			// resets position, angle and fitness

	return aPopulation;
}
