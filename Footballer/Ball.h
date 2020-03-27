#pragma once

#include "consts.h"

class Ball :
	public Sprite
{
public:
	Ball(string aFile);
	~Ball();
	void reset(uint aPosX, uint aPosY, uint aAngle);

private:
	uint m_angle{ 0 };
	uint m_posY{ 0 };
	uint m_posX{ 0 };

	Texture* m_pTexture{ nullptr };
};

