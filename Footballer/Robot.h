#pragma once
#include "Tree.h"
#include "City.h"

class Robot
{
public:
	~Robot();
	void birth();
	void reset(uint aPosX, uint aPosY, uint aAngle);

private:
	long m_fitness{ 0 };
	Tree* m_start{ nullptr };
	uint m_angle{ 0 };
	uint m_posY{ 0 };
	uint m_posX{ 0 };
};

