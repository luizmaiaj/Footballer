#include "Robot.h"

Robot::Robot(string aFilename)
{
	m_texture = new Texture();
	m_texture->loadFromFile(aFilename);
	setTexture(*m_texture);
}

Robot::~Robot()
{
	if (m_start) delete m_start;
	if (m_texture) delete m_texture;
}

void Robot::birth()
{
	m_start = new Tree();

	m_start->setRoot(true);
	m_start->create(nullptr, 1); // creates the random tree
}

void Robot::reset(float aPosX, float aPosY, float aAngle)
{
	m_fitness = 0;
	m_posX = aPosX;
	m_posY = aPosY;
	m_angle = aAngle;

	m_cursor = m_start;

	setPosition(m_posX, m_posY);
}

bool Robot::update(unique_ptr<City>& pCity, float aDelta)
{
	if (!m_cursor)
	{
		resetTree();
	}

	switch (m_cursor->getInfo())
	{
	case LEAF::IFWALL:
		break;
	case LEAF::ALIGN:
		break;
	case LEAF::FRONT:
		move(pCity, LEAF::FRONT);
		break;
	case LEAF::BACK:
		move(pCity, LEAF::BACK);
		break;
	case LEAF::LEFT:
		m_angle += ANGLE;
		break;
	case LEAF::RIGHT:
		m_angle -= ANGLE;
		break;
	default:
		break;
	}
	
	m_cursor->run();
	m_cursor = m_cursor->getNext();

	return true;
}

void Robot::move(unique_ptr<City>& pCity, LEAF aLeaf)
{
	float angle = (aLeaf == LEAF::BACK) ? m_angle + 180 : m_angle;
	float PosX = m_posX + (float) cos(angle * PI_180);
	float PosY = m_posY - (float) sin(angle * PI_180);

	if (!pCity->collidesEnvironment(PosX, PosY))
	{
		m_posX = PosX;
		m_posY = PosY;
	}

	setPosition(m_posX, m_posY);
}

void Robot::resetTree()
{
	m_start->reset();

	m_cursor = m_start;
}
