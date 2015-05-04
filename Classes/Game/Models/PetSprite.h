#ifndef __llgame__Pet__
#define __llgame__Pet__
#include "cocos2d.h"

using namespace cocos2d;
class PetSprite :public Node
{
public:
	int type;//宠物编号
	bool canRemove;//是否要消除
	Sprite * pet;
	CREATE_FUNC(PetSprite);
	static PetSprite *createPetSprite(int type);
	bool init();
};
#endif
