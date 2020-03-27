#pragma once

#include "consts.h"

constexpr uint BLOCKS = 9;

class City
{
public:
	City(uint aWidth, uint aHeight);
	void draw();
	bool collidesEnvironment(uint PosX, uint PosY);

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

