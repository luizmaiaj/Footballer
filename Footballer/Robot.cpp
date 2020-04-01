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
	m_fit = 0;
	m_unfit = 0;
	m_wallHit = 0;
	m_ballHit = 0;
	m_cursor = m_start;
}

bool Robot::execute(float aDelta)
{
	m_pBall->move();

	if (m_moves >= MOVES)
		return false;

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
		move(LEAF::FRONT); // increase fitness if moving towards the ball, reduce if moving away
		break;
	case LEAF::BACK:
		move(LEAF::BACK);
		break;
	case LEAF::LEFT:
		turn(LEAF::LEFT);
		break;
	case LEAF::RIGHT:
		turn(LEAF::RIGHT);
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
	*aSon = new Robot(this, m_pEnv); // create copy of this
	*aDaughter = new Robot(aMother, m_pEnv); // create copy of mother

	unsigned long lSon{ (rand() % ((*aSon)->getSize() - 1)) + 2 }; // select first point for the crossing
	unsigned long lDaughter{ (rand() % ((*aDaughter)->getSize() - 1)) + 2 };  // select second point for the crossing

	Tree* pCrossSon = (*aSon)->getRoot()->getPoint(lSon);
	Tree* pCrossDaughter = (*aDaughter)->getRoot()->getPoint(lDaughter);

	(*aSon)->getRoot()->setPoint(pCrossDaughter, lSon);
	(*aDaughter)->getRoot()->setPoint(pCrossSon, lDaughter);
}

void Robot::mutate(Robot** aSon)
{
	*aSon = new Robot(this, m_pEnv); // create copy

	unsigned long lSon{ (rand() % ((*aSon)->getSize() - 1)) + 2 }; // select a point for the mutation

	Tree* pMutateSon = (*aSon)->getRoot()->getPoint(lSon); // get pointer to mutation point

	pMutateSon->mutate();
}

void Robot::move(LEAF aLeaf)
{
	float angle = (aLeaf == LEAF::BACK) ? m_angle + 180 : m_angle;
	float posX = getPosition().x + (float)cos(angle * C_PI_180);
	float posY = getPosition().y - (float)sin(angle * C_PI_180);
	float distance = ballDistance();

	if (m_pEnv->collision(posX, posY)) // wall hit
		m_wallHit += 1;
	else
		setPosition(posX, posY);

	if (distance > ballDistance()) // if distance reduced
		m_fit += distance;
	else
		m_unfit += distance;

	hitBall();
}

void Robot::turn(LEAF aLeaf)
{
	m_angle = rebaseAngle(m_angle);

	float diff = abs(getBallAngle() - m_angle);

	if(aLeaf == LEAF::LEFT)
		m_angle += ANGLE; // increase fitness if turning into the direction oft the ball
	else
		m_angle -= ANGLE;

	if (abs(getBallAngle() - m_angle) < diff)
		m_fit += ANGLE;
	else
		m_unfit += ANGLE;
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
	m_angle = getBallAngle();
}

void Robot::hitBall()
{
	if (ballDistance() <= HITDISTANCE)
	{
		m_pBall->hit(m_angle);
		m_ballHit += 1;
	}
}

void Robot::resetTree()
{
	m_start->reset(false, m_size);

	m_cursor = m_start;
}

float Robot::getBallAngle()
{
	float dy = m_pBall->getPosition().y - getPosition().y;
	float dx = m_pBall->getPosition().x - getPosition().x;
	float angle = (float)C_180_PI * atan(dy / dx);

	if (dx >= 0)
		angle = 360 - angle;
	else
		angle = 180 - angle;

	return rebaseAngle(angle);
}

float Robot::rebaseAngle(float aAngle)
{
	int remainder = (int)aAngle;
	float save = aAngle - remainder;
	remainder %= 360;
	aAngle = remainder + save;
	return aAngle;
}

string Robot::getString()
{
	return m_start->getString();
}

float Robot::ballDistance()
{
	float dy = m_pBall->getPosition().y - getPosition().y;
	float dx = m_pBall->getPosition().x - getPosition().x;

	return sqrt((dy * dy) + (dx * dx));
}

void Robot::updateFitness()
{
	m_fitness = ((10 * m_fit) + (1000 * m_ballHit)) - ((10 * m_unfit) + (50 * m_wallHit));
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
