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
	void reset(float aPosX, float aPosY, float aAngle);
	bool update(float aDelta);
	void cross(Robot* aMother, Robot** aSon, Robot** aDaughter);
	float getFitness() { return m_fitness; };
	unsigned long getSize() { return m_size; };
	Tree* getRoot() { return m_start; };

private:
	void move(LEAF aLeaf);
	void resetTree();

	float m_fitness{ 0 };
	Tree* m_start{ nullptr };
	Tree* m_cursor{ nullptr };
	float m_angle{ 0 };
	unsigned long m_moves{ 0 };
	unsigned long m_size{ 0 };

	Texture* m_texture{ nullptr };
};

