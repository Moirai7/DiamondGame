//
//  DiamondSprite.cpp
//  ttgame
//
//  Created by zhao on 14-9-3.
//
//

#include "DiamondSprite.h"

#define TOTAL_SUSHI (7)

static const char *sushiNormal[TOTAL_SUSHI] = {
	"Diamond1.png",
	"Diamond2.png",
	"Diamond3.png",
	"Diamond4.png",
	"Diamond5.png",
    "Diamond6.png",
	"Diamond7.png"
};
static const char *sushiVertical[TOTAL_SUSHI] = {
	"Diamond1v.png",
	"Diamond2v.png",
	"Diamond3v.png",
	"Diamond4v.png",
	"Diamond5v.png",
    "Diamond6v.png",
	"Diamond7v.png"
};
static const char *sushiHorizontal[TOTAL_SUSHI] = {
	"Diamond1h.png",
	"Diamond2h.png",
	"Diamond3h.png",
	"Diamond4h.png",
	"Diamond5h.png",
    "Diamond6h.png",
	"Diamond7h.png"
};
float DiamondSprite::getContentWidth()
{
    static float itemWidth = 0;
    if (0 == itemWidth) {
        Sprite *sprite = CCSprite::createWithSpriteFrameName(sushiNormal[0]);
        itemWidth = sprite->getContentSize().width;
    }
    return itemWidth;
}

DiamondSprite::DiamondSprite()
: m_col(0)
, m_row(0)
, m_imgIndex(0)
, m_isNeedRemove(false)
, m_ignoreCheck(false)
, m_displayMode(DISPLAY_MODE_NORMAL)
{
}

DiamondSprite *DiamondSprite::create(int row, int col)
{
	DiamondSprite *sushi = new DiamondSprite();
	sushi->m_row = row;
	sushi->m_col = col;
    sushi->m_imgIndex = random() % TOTAL_SUSHI;
    sushi->initWithSpriteFrameName(sushiNormal[sushi->m_imgIndex]);
	sushi->autorelease();
	return sushi;
}

void DiamondSprite::setDisplayMode(DisplayMode mode)
{
    m_displayMode = mode;
    
    SpriteFrame *frame;
    switch (mode) {
        case DISPLAY_MODE_VERTICAL:
            frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(sushiVertical[m_imgIndex]);
            break;
        case DISPLAY_MODE_HORIZONTAL:
            frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(sushiHorizontal[m_imgIndex]);
            break;
        default:
            return;
    }
    setDisplayFrame(frame);
}