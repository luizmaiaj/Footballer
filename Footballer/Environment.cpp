#include "Environment.h"

Environment::Environment()
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
void Environment::drawbox(uint PosX, uint PosY, uint size)
{
	for (uint i = PosX; i < (PosX + size); i++)
		for (uint j = PosY; j < (PosY + size); j++)
			m_matriz[i][j] = 1;
}

bool Environment::collision(float aPosX, float aPosY)
{
	return (m_matriz[(uint)aPosX][(uint)aPosY] == 1);
}

void Environment::reflection(float aPosX, float aPosY, float& aAngle)
{
	bool bUp = false, bDown = false, bLeft = false, bRight = false;
	WALL wallDir = WALL::TOP;

	bUp = m_matriz[(uint)aPosX][(uint)(((aPosY - 1) < 1) ? 0 : aPosY - 1)] == 1;
	bDown = m_matriz[(uint)aPosX][(uint)(((aPosY + 1) > (HEIGHT - 2)) ? HEIGHT - 1 : aPosY + 1)] == 1;
	bLeft = m_matriz[(uint)(((aPosX - 1) < 1) ? 0 : aPosX - 1)][(uint)aPosX] == 1;
	bRight = m_matriz[(uint)(((aPosX + 1) > (WIDTH - 2)) ? WIDTH - 1 : aPosX + 1)][(uint)aPosX] == 1;

	int remainder = (int)aAngle;
	float save = aAngle - remainder;
	remainder %= 360;
	aAngle = remainder + save;

	if (!bUp) // should reflect up
	{
		if (aAngle < 90)
			aAngle -= 90;
		else
			aAngle += 90;
	}
	if (!bDown) // should reflect down
	{
		if (aAngle < 270)
			aAngle -= 90;
		else
			aAngle += 90;
	}
	if (!bLeft) // should reflect left
	{
		if (aAngle < 0)
			aAngle += 90;
		else
			aAngle -= 90;
	}
	if (!bRight) // should reflect right
	{
		if (aAngle < 180)
			aAngle += 90;
		else
			aAngle -= 90;
	}
}

// generates random coordinates and angle
void Environment::genCoord(float& aPosX, float& aPosY, float& aAngle)
{
	float aMin = 1;
	float aMax = HEIGHT - (1 + aMin);

	do
	{
		aPosX = (rand() % uint(aMax)) + aMin;

		aPosY = (rand() % uint(aMax)) + aMin;
	} while (collision(aPosX, aPosY));

	aAngle = ANGLE * (rand() % M_360_ANGLE);
}