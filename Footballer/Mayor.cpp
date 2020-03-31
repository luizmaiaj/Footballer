#include "Mayor.h"

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
			delete pR;
			m_robots.pop_back();
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

bool Mayor::update(float aDelta)
{
	bool bEnd{ false };

	for (itRobot it = m_robots.begin(); it != m_robots.end(); it++)
	{
		Robot* pR = *it;

		if (pR->execute(aDelta))
			bEnd = true;
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

uint Mayor::crossPopulation()
{
	float fMax{ 0 };
	float fTemp{ 0 };
	float fAvg{ 0 };

	// get maximum fitness
	for (itRobot it = m_robots.begin(); it != m_robots.end(); it++)
	{
		Robot* pR = *it;

		fTemp = pR->getFitness();

		fAvg += fTemp;

		if (fTemp > fMax) fMax = fTemp;
	}

	fAvg /= m_robots.size();

	printf("%d: Max: %.2f; Avg: %.2f\n", m_generation, fMax, fAvg);
	m_generation++;

	// remove 70% less performing
	fMax *= 0.7f;
	for (itRobot it = m_robots.begin(); it != m_robots.end(); )
	{
		Robot* pR = *it;

		if (pR->getFitness() < fMax)
		{
			it = m_robots.erase(it);
			delete pR;
		}
		else
		{
			it++;
		}
	}

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

	createPopulation(POPULATION);
	resetPopulation();

	return POPULATION;
}
