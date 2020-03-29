#pragma once

#include "consts.h"

class Ball :
	public Sprite
{
public:
	Ball(string aFile);
	~Ball();
	void reset(float aPosX, float aPosY, float aAngle);

private:
	float m_angle{ 0 };
	float m_posY{ 0 };
	float m_posX{ 0 };

	Texture* m_pTexture{ nullptr };
};

