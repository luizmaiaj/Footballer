#pragma once
#include "Robot.h"
#include "Ball.h"
#include <list>

typedef list<Robot*> listRobot;
typedef list<Robot*>::iterator itRobot;

using namespace std;

class Mayor
{
public:
	Mayor();
	~Mayor();
	uint loadPopulation(); // reads the population from files
	uint createPopulation(uint aPopulation, uint aCrossing);
	uint savePopulation();
	uint crossPopulation();
	bool update(float aDelta);
	void draw(RenderWindow* pWindow);

private:
	uint createPopulation(uint aPopulation);
	void resetPopulation();

	listRobot m_robots;
	Environment* m_pEnv{ nullptr };
	uint m_generation{ 0 };
	float m_lastMax{ 0.f };
};

