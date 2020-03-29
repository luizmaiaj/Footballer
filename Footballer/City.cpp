#include "City.h"

City::City()
{
	initialiseSprites();
	
	initialiseEnvironment();
}

//*******************************
//* RECEBE MATRIZ E A PREENCHE CONFORME O AMBIENTE INICIAL
//*******************************
void City::initialiseEnvironment()
{
	for (uint lin = 0; lin < HEIGHT; lin++)
	{
		for (uint col = 0; col < WIDTH; col++)
		{
			if (lin == 0 || lin == (HEIGHT - 1) || col == 0 || col == (WIDTH - 1))
				m_matriz[lin][col] = 1;
			else
				m_matriz[lin][col] = 0;
		}
	}

	drawbox(25, 25, 16);
	drawbox(25, 91, 16);
	drawbox(25, 160, 16);
	drawbox(91, 25, 16);
	drawbox(91, 91, 16);
	drawbox(91, 160, 16);
	drawbox(160, 25, 16);
	drawbox(160, 91, 16);
	drawbox(160, 160, 16);
}

//*******************************************
//* DESENHA OBSTACULO NA MATRIZ DO AMBIENTE *
//*******************************************
void City::drawbox(uint PosX, uint PosY, uint size)
{
	for (uint i = PosX; i < (PosX + size); i++)
		for (uint j = PosY; j < (PosY + size); j++)
			m_matriz[i][j] = 1;
}

bool City::collidesEnvironment(float PosX, float PosY)
{
	return (m_matriz[(uint) PosX][(uint) PosY] == 1);
}

Sprite* City::getBG()
{
	return m_pSpriteBG;
}

void City::getBlocks(Sprite* pSpriteBlock[BLOCKS])
{
	for(uint i = 0; i < BLOCKS; i++)
		pSpriteBlock[i] = m_pSpriteBlock[i];
}

void City::initialiseSprites()
{
	m_pTextureBG = new Texture();
	m_pTextureBG->loadFromFile("img/background.png");
	m_pSpriteBG = new Sprite();
	m_pSpriteBG->setTexture(*m_pTextureBG);

	m_pTextureBlock = new Texture();
	m_pTextureBlock->loadFromFile("img/block.png");

	for (uint i = 0; i < BLOCKS; i++)
	{
		m_pSpriteBlock[i] = new Sprite();
		m_pSpriteBlock[i]->setTexture(*m_pTextureBlock);
		m_pSpriteBlock[i]->setOrigin(0.f, 0.f);
	}

	m_pSpriteBlock[0]->setPosition(25.f, 25.f);
	m_pSpriteBlock[1]->setPosition(25.f, 91.f);
	m_pSpriteBlock[2]->setPosition(25.f, 160.f);
	m_pSpriteBlock[3]->setPosition(91.f, 25.f);
	m_pSpriteBlock[4]->setPosition(91.f, 91.f);
	m_pSpriteBlock[5]->setPosition(91.f, 160.f);
	m_pSpriteBlock[6]->setPosition(160.f, 25.f);
	m_pSpriteBlock[7]->setPosition(160.f, 91.f);
	m_pSpriteBlock[8]->setPosition(160.f, 160.f);
}
