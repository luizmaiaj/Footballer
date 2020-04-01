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
	void mutate(Robot** aSon);
	float getFitness() { return m_fitness; };
	unsigned long getSize() { return m_size; };
	Tree* getRoot() { return m_start; };
	Sprite* getBall() { return m_pBall; };
	string getString();
	float ballDistance();
	void updateFitness();

private:
	void initTexture();
	void initBall();
	void resetBall();

	void move(LEAF aLeaf);
	void turn(LEAF aLeaf);
	bool ifwall();
	void align();
	void hitBall();
	void resetTree();

	float getBallAngle();
	float rebaseAngle(float aAngle);

	Ball* m_pBall{ nullptr };

	float m_startX{ 0 };
	float m_startY{ 0 };
	Tree* m_start{ nullptr };
	Tree* m_cursor{ nullptr };
	float m_angle{ 0 };
	unsigned long m_moves{ 0 };
	unsigned long m_size{ 0 };

	Texture* m_texture{ nullptr };
	Environment* m_pEnv{ nullptr };

	// variables to calculate fitness
	float m_fitness{ 0 }; // resulting fitness
	float m_fit{ 0 }; // approaching ball
	float m_unfit{ 0 }; // distancing ball
	float m_wallHit{ 0 }; // wall hits
	float m_ballHit{ 0 }; // ball hits
};

