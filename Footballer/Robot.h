#pragma once
#include "Tree.h"
#include "Ball.h"

class Robot:
	public Sprite
{
public:
	Robot(Environment* pEnv);
	Robot(Robot* pParent, Environment* pEnv);
	~Robot();
	void birth();
	void reset();
	bool execute(float aDelta);
	void cross(Robot* aMother, Robot** aSon, Robot** aDaughter);
	float getFitness() { return m_fitness; };
	unsigned long getSize() { return m_size; };
	Tree* getRoot() { return m_start; };
	Sprite* getBall() { return m_pBall; };
	string getString();

private:
	void initTexture();
	void initBall();
	void resetBall();

	void move(LEAF aLeaf);
	bool ifwall();
	void align();
	void hitBall();
	void resetTree();

	Ball* m_pBall{ nullptr };

	float m_fitness{ 0 };
	float m_startX{ 0 };
	float m_startY{ 0 };
	Tree* m_start{ nullptr };
	Tree* m_cursor{ nullptr };
	float m_angle{ 0 };
	unsigned long m_moves{ 0 };
	unsigned long m_size{ 0 };

	Texture* m_texture{ nullptr };
	Environment* m_pEnv{ nullptr };
};

