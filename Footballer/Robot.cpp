#include "Robot.h"

Robot::Robot()
{
	initialiseEnvironment();

	m_texture = new Texture();
	m_texture->loadFromFile("img/mini_robot.png");
	setTexture(*m_texture);
}

Robot::Robot(Robot* pParent)
{
	initialiseEnvironment();

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

void Robot::reset()
{
	//srand((unsigned int)time(0));

	float PosX{ 0 }, PosY{ 0 };
	float aMin = 1;
	float aMax = HEIGHT - (1 + aMin);

	do
	{
		PosX = (rand() % uint(aMax)) + aMin;

		PosY = (rand() % uint(aMax)) + aMin;
	} while (collidesEnvironment(PosX, PosY));

	m_startX = PosX;
	m_startY = PosY;

	m_fitness = 0;
	m_moves = 0;
	m_angle = ANGLE * (rand() % M_360_ANGLE);
	m_cursor = m_start;

	setPosition(PosX, PosY);
	resetTree();
}

bool Robot::execute(float aDelta)
{
	if (m_moves >= MOVES)
	{
		m_fitness += abs(m_startX - getPosition().x) + abs(m_startY - getPosition().y);
		return false;
	}

	m_moves++;

	if (!m_cursor)
		resetTree();

	LEAF direction = LEAF::PROGN3;

	switch (m_cursor->getInfo())
	{
	case LEAF::IFWALL:
		if (ifwall())
			direction = LEAF::LEFT;
		else
			direction = LEAF::RIGHT;
		break;
	case LEAF::ALIGN:
		break;
	case LEAF::FRONT:
		move(LEAF::FRONT);
		break;
	case LEAF::BACK:
		move(LEAF::BACK);
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
	m_cursor = m_cursor->getNext(direction);

	return true;
}

void Robot::cross(Robot* aMother, Robot** aSon, Robot** aDaughter)
{
	//srand((unsigned int)time(0));

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

	float posX = getPosition().x + (float)cos(angle * PI_180);
	float posY = getPosition().y - (float)sin(angle * PI_180);

	if (collidesEnvironment(posX, posY))
		m_fitness -= 4;
	else
	{
		m_fitness += (aLeaf == LEAF::BACK) ? .5f : 2.f;
		setPosition(posX, posY);
	}
}

bool Robot::ifwall()
{
	float posY = getPosition().y - (2 * (float)sin(PI * m_angle * PI_180));
	float posX = getPosition().x + (2 * (float)cos(PI * m_angle * PI_180));

	if (collidesEnvironment(posX, posY) || posY < 1 || posY > (HEIGHT - 2) || posX < 1 || posY > (WIDTH - 2))
		return true;

	return false;
}

void Robot::resetTree()
{
	m_start->reset(false);

	m_cursor = m_start;
}

// RECEBE MATRIZ E A PREENCHE CONFORME O AMBIENTE INICIAL
void Robot::initialiseEnvironment()
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

// DESENHA OBSTACULO NA MATRIZ DO AMBIENTE
void Robot::drawbox(uint PosX, uint PosY, uint size)
{
	for (uint i = PosX; i < (PosX + size); i++)
		for (uint j = PosY; j < (PosY + size); j++)
			m_matriz[i][j] = 1;
}

bool Robot::collidesEnvironment(float PosX, float PosY)
{
	return (m_matriz[(uint)PosX][(uint)PosY] == 1);
}
