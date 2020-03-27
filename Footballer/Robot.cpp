#include "Robot.h"

Robot::~Robot()
{
	if (m_start) delete m_start;
}

void Robot::birth()
{
	m_start = new Tree();

	m_start->setRoot(true);
	m_start->create(m_start, 1);				// creates the random tree
}

void Robot::reset(uint aPosX, uint aPosY, uint aAngle)
{
	m_fitness = 0;
	m_posX = aPosX;
	m_posY = aPosY;
	m_angle = aAngle;
}
