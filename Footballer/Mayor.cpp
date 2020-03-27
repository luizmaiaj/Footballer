#include "Mayor.h"

Mayor::Mayor()
{
}

uint Mayor::loadPopulation()
{
	return 0; // to be implemented when we have sample files
}

uint Mayor::createPopulation(uint aPopulation)
{
	for (uint i = 0; i < aPopulation; i++)
	{
		Robot* pR = new Robot(); // new robot

		pR->birth(); // allocate the random tree

		m_robots.push_back(pR); // store
	}

	return aPopulation;
}

uint Mayor::testPopulation(City& aCity, uint aPopulation, uint aGenerations, uint aCrossing)
{
	for (uint iPop = 0; iPop < aPopulation; iPop++)
	{
		for (uint iGen = 0; iGen < aGenerations; iGen++)
		{

		}
	}

	return uint();
}
