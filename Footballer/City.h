#pragma once

#include <SFML/Graphics.hpp>
#include "consts.h"

constexpr uint BLOCKS = 9;

using namespace sf;

class City
{
public:
	City(uint aWidth, uint aHeight);
	void draw();

	RenderWindow* m_pWindow{ nullptr };

private:
	void initialiseSprites(uint aWidth, uint aHeight);
	void initialiseMessages();
	void initialiseTime();
	void initialiseEnvironment();
	void drawbox(uint lin, uint col, uint size);
	
	Texture* m_pTextureBG{ nullptr };
	Sprite* m_pSpriteBG{ nullptr };
	Texture* m_pTextureBlock{ nullptr };
	Sprite* m_pSpriteBlock[BLOCKS];
	uchar m_matriz[HEIGHT][WIDTH];
};

