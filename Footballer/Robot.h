#pragma once
#include "Tree.h"

class Robot:
	public Sprite
{
public:
	Robot();
	Robot(Robot* pParent);
	~Robot();
	void birth();
	void reset();
	bool execute(float aDelta);
	void cross(Robot* aMother, Robot** aSon, Robot** aDaughter);
	float getFitness() { return m_fitness; };
	unsigned long getSize() { return m_size; };
	Tree* getRoot() { return m_start; };

private:
	bool collidesEnvironment(float PosX, float PosY);
	void initialiseEnvironment();
	void drawbox(uint lin, uint col, uint size);

	void move(LEAF aLeaf);
	bool ifwall();
	void resetTree();

	float m_fitness{ 0 };
	float m_startX{ 0 };
	float m_startY{ 0 };
	Tree* m_start{ nullptr };
	Tree* m_cursor{ nullptr };
	float m_angle{ 0 };
	unsigned long m_moves{ 0 };
	unsigned long m_size{ 0 };

	Texture* m_texture{ nullptr };
	uchar m_matriz[HEIGHT][WIDTH];
};

