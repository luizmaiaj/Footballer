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
	void randomPosition(float aMin, float aMax, float& PosX, float& PosY);
	bool update(float aDelta);
	void draw(RenderWindow* pWindow);

	bool collidesEnvironment(float PosX, float PosY);

private:
	uint createPopulation(uint aPopulation);
	void resetPopulation();

	void initialiseEnvironment();
	void drawbox(uint lin, uint col, uint size);

	listRobot m_robots;
	uchar m_matriz[HEIGHT][WIDTH];
};

