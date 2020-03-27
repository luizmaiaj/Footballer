// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "Mayor.h"

using namespace std;

int main(void)
{
	unique_ptr<Mayor> pM(new Mayor());
	unique_ptr<City> pC(new City(WIDTH, HEIGHT));

	//mayor.loadPopulation();

	pM->createPopulation(POPULATION);

	// The main game loop
	while (pC->m_pWindow->isOpen())
	{
		if (Keyboard::isKeyPressed(Keyboard::Escape)) pC->m_pWindow->close(); // Handle the player quitting

		pC->draw();
	}

	return(0);
}
