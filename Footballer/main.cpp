// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "Mayor.h"

using namespace std;

int main(void)
{
	RenderWindow* pWindow{ nullptr };
	unique_ptr<Mayor> pM(new Mayor());
	unique_ptr<City> pC(new City());
	unique_ptr<Ball> pB(new Ball("img/mini_ball.png"));

	float PosX{ 0 }, PosY{ 0 };

	// The main game loop
	STATE state{ STATE::WAITING };
	Clock clock; // Here is our clock for timing everything
	Time totalTime{ Time::Zero }; // How long has the PLAYING state been active
	pWindow = new RenderWindow(VideoMode(WIDTH, HEIGHT), "Footballer", Style::Default); // window

	uint currentRobot{ 0 };

	while (pWindow->isOpen())
	{
		Time dt = clock.restart(); // Update the delta time
		totalTime += dt; // Update the total game time

		Event event;
		while (pWindow->pollEvent(event)) // Handle events
		{
			switch (event.type)
			{
			case Event::KeyPressed:
				if (event.key.code == Keyboard::Escape) pWindow->close(); //close window
				
				if (state == STATE::WAITING)
				{
					if (event.key.code == Keyboard::Return) // Restart while paused
					{
						pM->createPopulation(pC, POPULATION, CROSSING);

						pM->randomPosition(pC, 1, HEIGHT - 1, PosX, PosY);
						pB->reset(PosX, PosY, ANGLE * (rand() % M_360_ANGLE));

						state = STATE::SIMULATING;
						clock.restart(); // Reset the clock so there isn't a frame jump
					}
				}
				break;
			default:
				break;
			}
		}

		if (state == STATE::SIMULATING)
		{
			float delta = dt.asSeconds();
			pM->update(pC, delta);

			pWindow->clear(); // Clear everything from the last frame
			pWindow->draw(*pC->getBG()); // background

			Sprite* pSpriteBlock[BLOCKS];
			pC->getBlocks(pSpriteBlock);
			for (uint i = 0; i < BLOCKS; i++)
				pWindow->draw(*pSpriteBlock[i]);

			pM->drawRobots(pWindow);

			pWindow->display(); // Show everything we just drew
		}		
	}

	return(0);
}
