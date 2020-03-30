#include "Ball.h"

Ball::Ball(Environment* pEnv)
{
	m_pEnv = pEnv;

	m_pTexture = new Texture();
	m_pTexture->loadFromFile("img/mini_robot.png");

	setTexture(*m_pTexture);
}

Ball::~Ball()
{
	if (m_pTexture) delete m_pTexture;
}

void Ball::move()
{
	if (m_moves <= 0) return;

	m_moves--;

	float posX = getPosition().x + (float)cos(m_angle * C_PI_180);
	float posY = getPosition().y - (float)sin(m_angle * C_PI_180);

	if(m_pEnv->collision(posX, posY))
		m_pEnv->reflection(posX, posY, m_angle); // will correct the ball's angle

	setPosition(posX, posY);
}

void Ball::reset(float aPosX, float aPosY)
{
	m_startX = aPosX;
	m_startY = aPosY;

	setPosition(aPosX, aPosY);
}
