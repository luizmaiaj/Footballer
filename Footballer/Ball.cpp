#include "Ball.h"

Ball::Ball(string aFile)
{
	m_pTexture = new Texture();
	m_pTexture->loadFromFile(aFile);

	setTexture(*m_pTexture);
}

Ball::~Ball()
{
	if (m_pTexture) delete m_pTexture;
}

void Ball::reset(float aPosX, float aPosY, float aAngle)
{
	m_posX = aPosX;
	m_posY = aPosY;
	m_angle = aAngle;
}
