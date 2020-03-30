#include "Robot.h"

Robot::Robot(Environment* pEnv)
{
	m_pEnv = pEnv;

	initTexture();

	initBall();
}

Robot::Robot(Robot* pParent, Environment* pEnv)
{
	m_pEnv = pEnv;

	initTexture();

	initBall();

	m_start = new Tree();

	m_start->setRoot(true);
	m_start->create(nullptr, m_size, pParent->getRoot());
}

Robot::~Robot()
{
	if (m_start) delete m_start;
	if (m_texture) delete m_texture;
	if (m_pBall) delete m_pBall;
}

void Robot::birth()
{
	m_start = new Tree();

	m_start->setRoot(true);
	m_start->create(nullptr, m_size, nullptr); // creates the random tree
}

void Robot::reset()
{
	m_pEnv->genCoord(m_startX, m_startY, m_angle);
	setPosition(m_startX, m_startY);
	resetTree();
	resetBall();

	m_fitness = 0;
	m_moves = 0;
	m_cursor = m_start;
}

bool Robot::execute(float aDelta)
{
	m_pBall->move();

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
		align();
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
	*aSon = new Robot(this, m_pEnv);
	*aDaughter = new Robot(aMother, m_pEnv);

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

	float posX = getPosition().x + (float)cos(angle * C_PI_180);
	float posY = getPosition().y - (float)sin(angle * C_PI_180);

	if (m_pEnv->collision(posX, posY))
		m_fitness -= 4;
	else
	{
		m_fitness += (aLeaf == LEAF::BACK) ? .5f : 2.f;
		setPosition(posX, posY);
	}

	hitBall();
}

bool Robot::ifwall()
{
	float posY = getPosition().y - (2 * (float)sin(C_PI * m_angle * C_PI_180));
	float posX = getPosition().x + (2 * (float)cos(C_PI * m_angle * C_PI_180));

	if (m_pEnv->collision(posX, posY) || posY < 1 || posY > (HEIGHT - 2) || posX < 1 || posY > (WIDTH - 2))
		return true;

	return false;
}

void Robot::align()
{
	float dy = m_pBall->getPosition().y - getPosition().y;
	float dx = m_pBall->getPosition().x - getPosition().x;
	float angle = (float) C_180_PI * atan(dy / dx);

	if (dx >= 0)
		m_angle = 360 - angle;
	else
		m_angle = 180 - angle;
}

void Robot::hitBall()
{
	float dy = m_pBall->getPosition().y - getPosition().y;
	float dx = m_pBall->getPosition().x - getPosition().x;

	if (sqrt((dy * dy) + (dx * dx)) <= HITDISTANCE)
	{
		m_pBall->hit(m_angle);
		m_fitness += 500;

		// use random distance to move the ball and initial distance to robot to change fitness function
		/*ballmoves = (rand() % 46) + 5;  //BALLMOVE RECEBE NUMERO DE 5 a 50;
		if (!nbef)
		{
			unfit = n / Dini;
			nbef = n;
		}
		else
		{
			unfit += (n - nbef) / Dini;
			nbef = n;
		}*/
	}
}

void Robot::resetTree()
{
	m_start->reset(false);

	m_cursor = m_start;
}

void Robot::initTexture()
{
	m_texture = new Texture();
	m_texture->loadFromFile("img/mini_robot.png");
	setTexture(*m_texture);
}

void Robot::initBall()
{
	m_pBall = new Ball(m_pEnv);

	resetBall();
}

void Robot::resetBall()
{
	float posX{ 0 }, posY{ 0 }, angle{ 0 };

	m_pEnv->genCoord(posX, posY, angle);

	m_pBall->reset(posX, posY);
}
