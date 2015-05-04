//
//  MonsterSprite.h
//  
//  怪物有血量，打完就赢了
//  一局可能有多只怪物
//
//

#ifndef __llgame__Monster__
#define __llgame__Monster__
#include "cocos2d.h"

using namespace cocos2d;
class MonsterSprite:public Node
{
public:
    int number;//数字编号 怪物编号
    int blood;
    bool canRemove;//是否要消除
    Sprite * monster;
    CREATE_FUNC(MonsterSprite);
    static MonsterSprite *createMonsterSprite(int num,int blood);
	bool init();
};
#endif