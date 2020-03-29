#pragma once
#include "Tree.h"
#include "City.h"

class Robot:
	public Sprite
{
public:
	Robot(string aFilename);
	~Robot();
	void birth();
	void reset(float aPosX, float aPosY, float aAngle);
	bool update(unique_ptr<City>& pCity, float aDelta);

private:
	void move(unique_ptr<City>& pCity, LEAF aLeaf);
	void resetTree();

	long m_fitness{ 0 };
	Tree* m_start{ nullptr };
	Tree* m_cursor{ nullptr };
	float m_angle{ 0 };
	float m_posY{ 0 };
	float m_posX{ 0 };

	Texture* m_texture{ nullptr };
};

