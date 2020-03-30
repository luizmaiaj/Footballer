// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "Mayor.h"
#include "City.h"

using namespace std;

int main(void)
{
	RenderWindow* pWindow = new RenderWindow(VideoMode(WIDTH, HEIGHT), "Footballer", Style::Default); // window
	unique_ptr<Mayor> pM(new Mayor());
	unique_ptr<City> pC(new City());

	float PosX{ 0 }, PosY{ 0 };

	// The main game loop
	STATE state{ STATE::WAITING };
	Clock clock; // Here is our clock for timing everything
	Time totalTime{ Time::Zero }; // How long has the PLAYING state been active

	Sprite* pSpriteBlock[BLOCKS];
	pC->getBlocks(pSpriteBlock);

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
						pM->createPopulation(POPULATION, CROSSING);

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

			if (!pM->update(delta))
				state = STATE::CROSSING; // if all individuals have been executed till the limit MOVES

			pWindow->clear(); // Clear everything from the last frame
			pWindow->draw(*pC->getBG()); // background

			for (uint i = 0; i < BLOCKS; i++)
				pWindow->draw(*pSpriteBlock[i]);

			pM->draw(pWindow);

			pWindow->display(); // Show everything we just drew
		}
		else if (state == STATE::CROSSING)
		{
			srand((unsigned int)time(0)); // use new seed

			pM->crossPopulation();
			state = STATE::SIMULATING;
		}
	}

	return(0);
}
