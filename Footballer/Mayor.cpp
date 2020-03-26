#include "Mayor.h"

Mayor::Mayor(uint aPopulation)
{
	for (uint i = 0; i < aPopulation; i++)
	{
		Robot* pZ = new Robot(); // zombie sound
		m_robots.push_back(pZ);
	}
}

uint Mayor::loadPopulation()
{
	return 0; // to be implemented when we have sample files
}

uint Mayor::createPopulation()
{
	uint i{ 0 };

	for (itRobot it = m_robots.begin(); it != m_robots.end(); it++)
	{
		Robot* pRobot = *it;

		pRobot->birth();
		i++;
		//printf("\n");
	}

	return i;
}

uint Mayor::testPopulation(uint aPopulation, uint aGenerations, uint aCrossing)
{
	for (uint iPop = 0; iPop < aPopulation; iPop++)
	{
		for (uint iGen = 0; iGen < aGenerations; iGen++)
		{

		}
	}

	return uint();
}
