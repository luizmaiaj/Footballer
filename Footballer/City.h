#pragma once

#include "consts.h"

constexpr uint BLOCKS = 9;

class City
{
public:
	City();
	bool collidesEnvironment(float PosX, float PosY);
	Sprite* getBG();
	void getBlocks(Sprite* pSpriteBlock[BLOCKS]);

private:
	void initialiseSprites();
	void initialiseEnvironment();
	void drawbox(uint lin, uint col, uint size);
	
	Texture* m_pTextureBG{ nullptr };
	Sprite* m_pSpriteBG{ nullptr };
	Texture* m_pTextureBlock{ nullptr };
	Sprite* m_pSpriteBlock[BLOCKS];
	uchar m_matriz[HEIGHT][WIDTH];
};

