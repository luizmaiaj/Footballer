#pragma once

#include "consts.h"

constexpr uint BLOCKS = 9;

class City
{
public:
	City();
	Sprite* getBG();
	void getBlocks(Sprite* pSpriteBlock[BLOCKS]);

private:
	void initialiseSprites();
	
	Texture* m_pTextureBG{ nullptr };
	Sprite* m_pSpriteBG{ nullptr };
	Texture* m_pTextureBlock{ nullptr };
	Sprite* m_pSpriteBlock[BLOCKS];
};

