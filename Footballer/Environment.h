#pragma once

#include "consts.h"

class Environment
{
public:
	Environment();
	bool collision(float aPosX, float aPosY);
	void reflection(float& aPosX, float& aPosY, float& aAngle);
	void genCoord(float& aPosX, float& aPosY, float& aAngle);

private:
	void drawbox(uint PosX, uint PosY, uint size);
	uchar m_matriz[HEIGHT][WIDTH];
};

