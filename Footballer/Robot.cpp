#include "Robot.h"

void Robot::birth()
{
	m_start = new Tree();

	m_start->setRoot(true);
	m_start->create(m_start, 1);
}
