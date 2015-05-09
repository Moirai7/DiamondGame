#include "PetSprite.h"





PetSprite * PetSprite::createPetSprite(int type){
	PetSprite * nc = PetSprite::create();
	nc->type = type;
	nc->pet = Sprite::create(StringUtils::format("LevelSelection/thumb%d.png", type));
	nc->pet->retain();
	nc->canRemove = false;
	//    auto bg=LayerColor::create(Color4B(175,175,175,125),80, 80);
	//    nc->addChild(bg);
	//nc->monster->setContentSize(Size(80,80));


	return nc;
}
bool PetSprite::init(){
	if (!Node::init())
	{
		return false;
	}
	return true;
}
