#pragma once

#include "consts.h"
#include "Environment.h"

class Ball :
	public Sprite
{
public:
	Ball(Environment* pEnv);
	~Ball();
	void move();
	void hit(float aAngle) { m_angle = aAngle; m_moves = 40; };
	void reset(float aPosX, float aPosY);
	void getStart(float& aPosX, float& aPosY) { aPosX = m_startX; aPosY = m_startY; };

private:
	float m_startY{ 0 };
	float m_startX{ 0 };
	float m_angle{ 0 };

	unsigned long m_moves{ 0 };

	Texture* m_pTexture{ nullptr };
	Environment* m_pEnv{ nullptr };
};

