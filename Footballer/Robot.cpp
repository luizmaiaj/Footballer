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

	m_dir = ANGLE * (rand() % (360 / ANGLE));	// sets the initial angle
	m_col = (rand() % 198) + 1;					// sets the initial column or x coordinate
	m_lin = (rand() % 198) + 1;					// sets the initial line or y coordinate
}
