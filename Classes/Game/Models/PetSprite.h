#ifndef __llgame__Pet__
#define __llgame__Pet__
#include "cocos2d.h"

using namespace cocos2d;
class PetSprite :public Node
{
public:
	int type;//������
	bool canRemove;//�Ƿ�Ҫ����
	Sprite * pet;
	CREATE_FUNC(PetSprite);
	static PetSprite *createPetSprite(int type);
	bool init();
};
#endif
