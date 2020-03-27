// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "Mayor.h"

using namespace std;

int main(void)
{
	unique_ptr<Mayor> pM(new Mayor());
	unique_ptr<City> pC(new City(WIDTH, HEIGHT));
	unique_ptr<Ball> pB(new Ball("img/mini_ball.png"));



	pM->createPopulation(pC, POPULATION, CROSSING);

	uint PosX{ 0 }, PosY{ 0 };
	pM->randomPosition(pC, 1, HEIGHT - 1, PosX, PosY);
	pB->reset(PosX, PosY, ANGLE * (rand() % (360 / ANGLE)));

	// The main game loop
	while (pC->m_pWindow->isOpen())
	{
		if (Keyboard::isKeyPressed(Keyboard::Escape)) pC->m_pWindow->close(); // Handle the player quitting

		pC->draw();
	}

	return(0);
}
