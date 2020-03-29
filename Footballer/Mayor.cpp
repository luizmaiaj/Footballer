#include "Mayor.h"

Mayor::Mayor()
{
	initialiseEnvironment();
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
		Robot* pR = new Robot(); // new robot

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
		float PosX{ 0 }, PosY{ 0 };

		randomPosition(1, HEIGHT - 1, PosX, PosY);

		pR->reset(PosX, PosY, ANGLE * (rand() % M_360_ANGLE));
	}
}

void Mayor::randomPosition(float aMin, float aMax, float& PosX, float& PosY)
{
	do
	{
		aMax -= aMin;

		PosX = (rand() % uint(aMax)) + aMin;

		PosY = (rand() % uint(aMax)) + aMin;
	}
	while (collidesEnvironment(PosX, PosY));
}

bool Mayor::update(float aDelta)
{
	bool bEnd{ false };

	for (itRobot it = m_robots.begin(); it != m_robots.end(); it++)
	{
		Robot* pR = *it;

		float posX{ pR->getPosition().x };
		float posY{ pR->getPosition().y };

		if (pR->update(aDelta))
			bEnd = true;

		float nposX{ pR->getPosition().x };
		float nposY{ pR->getPosition().y };

		if (collidesEnvironment(nposX, nposY))
			pR->setPosition(posX, posY);
	}

	return bEnd;
}

void Mayor::draw(RenderWindow* pWindow)
{
	for (itRobot it = m_robots.begin(); it != m_robots.end(); it++)
	{
		Robot* pR = *it;

		pWindow->draw(*pR);
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
	float lMax{ 0 };
	float lTemp{ 0 };

	// get maximum fitness
	for (itRobot it = m_robots.begin(); it != m_robots.end(); it++)
	{
		Robot* pR = *it;

		lTemp = pR->getFitness();

		if (lTemp > lMax) lMax = lTemp;
	}

	// remove 70% less performing
	lMax *= 0.7f;
	for (itRobot it = m_robots.begin(); it != m_robots.end(); )
	{
		Robot* pR = *it;

		if (pR->getFitness() < lMax)
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

//*******************************
//* RECEBE MATRIZ E A PREENCHE CONFORME O AMBIENTE INICIAL
//*******************************
void Mayor::initialiseEnvironment()
{
	for (uint lin = 0; lin < HEIGHT; lin++)
	{
		for (uint col = 0; col < WIDTH; col++)
		{
			if (lin == 0 || lin == (HEIGHT - 1) || col == 0 || col == (WIDTH - 1))
				m_matriz[lin][col] = 1;
			else
				m_matriz[lin][col] = 0;
		}
	}

	drawbox(25, 25, 16);
	drawbox(25, 91, 16);
	drawbox(25, 160, 16);
	drawbox(91, 25, 16);
	drawbox(91, 91, 16);
	drawbox(91, 160, 16);
	drawbox(160, 25, 16);
	drawbox(160, 91, 16);
	drawbox(160, 160, 16);
}

//*******************************************
//* DESENHA OBSTACULO NA MATRIZ DO AMBIENTE *
//*******************************************
void Mayor::drawbox(uint PosX, uint PosY, uint size)
{
	for (uint i = PosX; i < (PosX + size); i++)
		for (uint j = PosY; j < (PosY + size); j++)
			m_matriz[i][j] = 1;
}

bool Mayor::collidesEnvironment(float PosX, float PosY)
{
	return (m_matriz[(uint)PosX][(uint)PosY] == 1);
}
