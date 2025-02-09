#include "Mayor.h"
#include <stdio.h>

Mayor::Mayor()
{
	m_pEnv = new Environment();
}

Mayor::~Mayor()
{
	for (itRobot it = m_robots.begin(); it != m_robots.end(); it++)
	{
		Robot* pR = *it;

		if (pR) delete pR;
	}

	if (m_pEnv) delete m_pEnv;
}

uint Mayor::loadPopulation()
{
	return 0; // to be implemented when we have sample files
}

uint Mayor::createPopulation(uint aPopulation)
{
	if (m_robots.size() == aPopulation)
		return aPopulation;

	if (m_robots.size() > aPopulation)
	{
		do
		{
			Robot* pR = m_robots.back();
			m_robots.pop_back();
			delete pR;
		} while (m_robots.size() > aPopulation);
	}

	for (uint i = m_robots.size(); i < aPopulation; i++)
	{
		Robot* pR = new Robot(m_pEnv); // new robot

		pR->birth(); // allocate the random tree

		m_robots.push_back(pR); // store
	}

	return aPopulation;
}

void Mayor::resetPopulation()
{
	for (itRobot it = m_robots.begin(); it != m_robots.end(); it++)
	{
		Robot* pR = *it;
		pR->reset();
	}
}

// gets average and updates last maximum
float Mayor::getAvg()
{
	float fTemp = 0.f;
	float fAvg = 0.f;

	// get maximum fitness
	for (itRobot it = m_robots.begin(); it != m_robots.end(); it++)
	{
		Robot* pR = *it;

		fTemp = pR->getFitness();

		fAvg += fTemp;

		if (fTemp > m_lastMax) m_lastMax = fTemp;
	}

	fAvg /= m_robots.size();

	return fAvg;
}

float Mayor::eraseWorst(float aCut)
{
	// remove 70% less performing
	float fCut = m_lastMax * aCut;
	float fAvg = 0.f;
	float fTemp = 0.f;
	for (itRobot it = m_robots.begin(); it != m_robots.end(); )
	{
		Robot* pR = *it;

		fTemp = pR->getFitness();

		if (fTemp < fCut)
		{
			it = m_robots.erase(it);
			delete pR;
		}
		else
		{
			fAvg += fTemp;
			it++;
		}
	}

	fAvg /= m_robots.size();

	return fAvg;
}

// creates offspring based on the remaining best individuals
void Mayor::createOffspring()
{
	// create offspring
	listRobot offspring;
	for (itRobot it = m_robots.begin(); it != m_robots.end(); )
	{
		Robot* pRFather = *it++;

		if (it == m_robots.end()) break; // leave if there are no sufficient parents

		Robot* pRMother = *it++;

		Robot* pSon{ nullptr };
		Robot* pDaughter{ nullptr };

		pRFather->cross(pRMother, &pSon, &pDaughter);

		offspring.push_back(pSon);
		offspring.push_back(pDaughter);
	}

	// add offsprings to the robot list
	for (itRobot it = offspring.begin(); it != offspring.end(); it++)
	{
		Robot* pR = *it;

		m_robots.push_back(pR);
	}
}

void Mayor::mutatePopulation(float aRate)
{
	// create offspring
	listRobot offspring;
	for (itRobot it = m_robots.begin(); it != m_robots.end(); it++)
	{
		Robot* pR = *it;

		if (pR->getFitness() > (m_lastMax * aRate))
		{
			Robot* pSon{ nullptr };

			pR->mutate(&pSon);

			offspring.push_back(pSon);
		}
	}

	// add offsprings to the robot list
	for (itRobot it = offspring.begin(); it != offspring.end(); it++)
	{
		Robot* pR = *it;

		m_robots.push_back(pR);
	}
}

bool Mayor::update(float aDelta)
{
	bool bEnd{ false };

	for (itRobot it = m_robots.begin(); it != m_robots.end(); it++)
	{
		Robot* pR = *it;

		if (pR->execute(aDelta))
			bEnd = true;
	}

	if (bEnd) // udpate the fitness if it's the end of the population execution
	{ 
		for (itRobot it = m_robots.begin(); it != m_robots.end(); it++)
		{
			Robot* pR = *it;

			pR->updateFitness();
		}
	}

	return bEnd;
}

void Mayor::draw(RenderWindow* pWindow)
{
	for (itRobot it = m_robots.begin(); it != m_robots.end(); it++)
	{
		Robot* pR = *it;

		pWindow->draw(*pR);

		pWindow->draw(*pR->getBall());
	}
}

uint Mayor::createPopulation(uint aPopulation, uint aCrossing)
{
	aPopulation = createPopulation(aPopulation);	// creates or completes a population with new individuals
	resetPopulation();							// resets position, angle and fitness

	return aPopulation;
}

uint Mayor::savePopulation()
{
	float fCut = m_lastMax * .7f;
	uint i = 0;
	char filename[256] = { '\0' };

	for (itRobot it = m_robots.begin(); it != m_robots.end(); it++)
	{
		sprintf_s(filename, sizeof(filename), "ind/ind%03d.foot", i++);

		Robot* pR = *it;

		if (pR->getFitness() > fCut)
		{
			uint size = pR->getSize();

			std::ofstream outputFile(filename);
			outputFile << pR->getString();
			outputFile.close();
		}
	}

	return uint();
}

uint Mayor::crossPopulation()
{
	float fAvg = getAvg();
	float fTemp = 0.f;

	printf("%d: Max: %.2f; Avg: %.2f", m_generation, m_lastMax, fAvg);
	m_generation++;

	fAvg = eraseWorst(0.5f);
	printf("; Best Avg: %.2f\n", fAvg);
	printf("After erase: %d\n", m_robots.size());

	createOffspring();
	printf("After cross: %d\n", m_robots.size());

	mutatePopulation(0.7f);
	printf("After mutate: %d\n", m_robots.size());

	createPopulation(POPULATION);
	resetPopulation();

	return POPULATION;
}
