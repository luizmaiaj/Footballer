#include "Robot.h"

Robot::Robot()
{
	m_texture = new Texture();
	m_texture->loadFromFile("img/mini_robot.png");
	setTexture(*m_texture);
}

Robot::Robot(Robot* pParent)
{
	m_texture = new Texture();
	m_texture->loadFromFile("img/mini_robot.png");
	setTexture(*m_texture);

	m_start = new Tree();

	m_start->setRoot(true);
	m_start->create(nullptr, m_size, pParent->getRoot());
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
	m_start->create(nullptr, m_size, nullptr); // creates the random tree
}

void Robot::reset(float aPosX, float aPosY, float aAngle)
{
	m_fitness = 0;
	m_moves = 0;
	m_angle = aAngle;
	m_cursor = m_start;

	setPosition(aPosX, aPosY);
	resetTree();
}

bool Robot::update(float aDelta)
{
	if (m_moves >= MOVES)
		return false;
	m_moves++;

	if (!m_cursor)
		resetTree();

	switch (m_cursor->getInfo())
	{
	case LEAF::IFWALL:
		break;
	case LEAF::ALIGN:
		break;
	case LEAF::FRONT:
		move(LEAF::FRONT);
		m_fitness += 2;
		break;
	case LEAF::BACK:
		move(LEAF::BACK);
		m_fitness += 2;
		break;
	case LEAF::LEFT:
		m_angle += ANGLE;
		m_fitness++;
		break;
	case LEAF::RIGHT:
		m_angle -= ANGLE;
		m_fitness++;
		break;
	default:
		break;
	}
	
	m_cursor->run();
	m_cursor = m_cursor->getNext();

	return true;
}

void Robot::cross(Robot* aMother, Robot** aSon, Robot** aDaughter)
{
	*aSon = new Robot(this);
	*aDaughter = new Robot(aMother);

	unsigned long lSon{ (rand() % ((*aSon)->getSize() - 1)) + 2 };
	unsigned long lDaughter{ (rand() % ((*aDaughter)->getSize() - 1)) + 2 };

	Tree* pCrossSon = (*aSon)->getRoot()->getPoint(lSon);
	Tree* pCrossDaughter = (*aDaughter)->getRoot()->getPoint(lDaughter);

	(*aSon)->getRoot()->setPoint(pCrossDaughter, lSon);
	(*aDaughter)->getRoot()->setPoint(pCrossSon, lDaughter);
}

void Robot::move(LEAF aLeaf)
{
	float angle = (aLeaf == LEAF::BACK) ? m_angle + 180 : m_angle;

	setPosition(getPosition().x + (float)cos(angle * PI_180), getPosition().y - (float)sin(angle * PI_180));
}

void Robot::resetTree()
{
	m_start->reset();

	m_cursor = m_start;
}
