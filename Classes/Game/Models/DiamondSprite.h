
//  钻石完成
//

#ifndef __ttgame__Diamond__
#define __ttgame__Diamond__

#include "cocos2d.h"
typedef enum {
    DISPLAY_MODE_NORMAL = 0,
    DISPLAY_MODE_HORIZONTAL,
    DISPLAY_MODE_VERTICAL,
} DisplayMode;
using namespace cocos2d;
class DiamondSprite:public Sprite
{
public:
	static DiamondSprite *create(int row, int col);
	int getCol(){return m_col;}
	int getRow(){return m_row;}
	static float getContentWidth();
	DiamondSprite();
    CC_SYNTHESIZE(int, m_row, Row);
    CC_SYNTHESIZE(int, m_col, Col);
    CC_SYNTHESIZE(int, m_imgIndex, ImgIndex);
    CC_SYNTHESIZE(bool, m_isNeedRemove, IsNeedRemove);
    CC_SYNTHESIZE(bool, m_ignoreCheck, IgnoreCheck);//新产生的4消寿司，本轮不被消除
    CC_SYNTHESIZE_READONLY(DisplayMode, m_displayMode, DisplayMode);
    void setDisplayMode(DisplayMode mode);
};
#endif /* defined(__ttgame__Card__) */
