#pragma once
#include "Robot.h"
#include "Ball.h"
#include <list>

typedef list<Robot*> listRobot;
typedef list<Robot*>::iterator itRobot;

class Mayor
{
public:
	Mayor();
	~Mayor();
	uint loadPopulation(); // reads the population from files
	uint createPopulation(uint aPopulation, uint aCrossing);
	uint crossPopulation();
	bool update(float aDelta);
	void draw(RenderWindow* pWindow);

private:
	uint createPopulation(uint aPopulation);
	void resetPopulation();

	listRobot m_robots;
	Environment* m_pEnv{ nullptr };
};

