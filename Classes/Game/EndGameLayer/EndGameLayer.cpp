#include "EndGameLayer.h"
#include "Game/SharedClass/GameConstants.h"
#include "Game/PlayScene/PlayScene.h"
#include "Game/GameStartScene/GameStartScene.h"
#include "Game/SharedClass/GameState.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;

#define ENDGAME_BACKGROUND "SocialUI/background_no_logo.png"
#define LEVELCOMPELTE_LABEL "SocialUI/levelComplete.png"
#define GAMEOVER_LABEL "SocialUI/gameOver.png"

Scene* EndGameLayer::scene()
{
	Scene *scene = CCScene::create();
	EndGameLayer *layer = EndGameLayer::create();
	scene->addChild(layer);
	return scene;
}

// on "init" you need to initialize your instance
bool EndGameLayer::init()
{
	//////////////////////////////
	// 1. super init first
	if (!cocos2d::Layer::init())
	{
		return false;
	}
	winSize = Director::getInstance()->getWinSize();
	setupGUI();
	this->setTouchEnabled(true);

	return true;
}

void EndGameLayer:: onEnter()
{
	//TODO
	//Director::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
	Layer::onEnter();
}

void EndGameLayer::setupGUI()
{
	int pet_ = GAME_SHARED_HANDLER->getPet();
	background = Sprite::create(StringUtils::format("Backgrounds/back_%d.jpg",pet_));
	background->setAnchorPoint(Point(0, 1));
	background->setPosition(Point(0, winSize.height));
	this->addChild(background,-1);

	MenuItemImage *restartMenuItem = MenuItemImage::create(BUTTON_RESTART_GAME, BUTTON_RESTART_GAME_PRESSED,CC_CALLBACK_1(EndGameLayer::restartGame,this) );
	MenuItemImage *quitMenuItem = MenuItemImage::create(BUTTON_QUIT_GAME, BUTTON_QUIT_GAME_PRESSED, CC_CALLBACK_1(EndGameLayer::quitGame,this) );
	MenuItemImage *nextMenuItem = MenuItemImage::create(BUTTON_NEXT_GAME, BUTTON_NEXT_GAME_PRESSED,CC_CALLBACK_1(EndGameLayer::nextGame,this) );

	restartMenuItem->setPosition(Vec2(winSize.width/2 - 150, winSize.height/2 - 250 ));
	restartMenuItem->setScale(0.5);
	
	nextMenuItem->setPosition(Vec2(winSize.width/2 - 150, winSize.height/2 - 250));
	nextMenuItem->setScale(0.5);

	quitMenuItem->setPosition(Vec2(winSize.width/2 + 150, winSize.height/2 - 250 ));
	quitMenuItem->setScale(0.5);

	GameState *gs = GAME_SHARED_HANDLER;

	if(gs->score == 1&&gs->getCurLevel()<gs->getTotalLevel()){
		Sprite *sprite = Sprite::create(GAMEOVER_LABEL);
		sprite->setPosition(Vec2(winSize.width/2, winSize.height/2 + 160));
		this->addChild(sprite, 26);

		Menu *menu = Menu::create(nextMenuItem,quitMenuItem,NULL);
		menu->setPosition(Vec2(0,0));
		background->addChild(menu);
	}
	else if(gs->getCurLevel()>=gs->getTotalLevel()&&gs->score == 1){
		Sprite *sprite = Sprite::create(LEVELCOMPELTE_LABEL);
		sprite->setPosition(Vec2(winSize.width/2, winSize.height/2 + 160));
		this->addChild(sprite, 26);

		quitMenuItem->setPosition(Vec2(winSize.width/2 , winSize.height/2 - 250 ));
		Menu *menu = Menu::create(quitMenuItem,NULL);
		menu->setPosition(Vec2(0,0));
		background->addChild(menu);
	}else{
		Sprite *sprite = Sprite::create(GAMEOVER_LABEL);
		sprite->setPosition(Vec2(winSize.width/2, winSize.height/2 + 160));
		this->addChild(sprite, 26);

		Menu *menu = Menu::create(restartMenuItem,quitMenuItem,NULL);
		menu->setPosition(Vec2(0,0));
		background->addChild(menu);
	}
}

void EndGameLayer::restartGame(Ref* r){
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("Audio/ButtonClick.mp3");
	GAME_SHARED_HANDLER->getGameInfo(*(GAME_SHARED_HANDLER->levelInfoDict),GAME_SHARED_HANDLER->getCurLevel());
	Scene *gameLayer = Play::createScene();
	Director::sharedDirector()->replaceScene(TransitionCrossFade::create(0.3,gameLayer));
}
void EndGameLayer::quitGame(Ref* r){

	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("Audio/ButtonClick.mp3");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Audio/MainMenuMusic.mp3");
	this->removeFromParent();
	Scene *scene = Scene::create();
	GameStart *playLayer = GameStart::create();
	scene->addChild(playLayer);
	Director::sharedDirector()->replaceScene(scene);
}
void EndGameLayer::nextGame(Ref* r){

	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("Audio/ButtonClick.mp3");
	//Scene *scene = Scene::create();
	//gameLayer->GameStart();
	// scene->addChild(gameLayer);
	//GAME_SHARED_HANDLER->getGameInfo(GAME_SHARED_HANDLER->levelInfoDict,GAME_SHARED_HANDLER->getCurLevel()+1);
	if(GAME_SHARED_HANDLER->getCurLevel()<GAME_SHARED_HANDLER->getTotalLevel()){
		GAME_SHARED_HANDLER->setCurLevel(GAME_SHARED_HANDLER->getCurLevel()+1);
		GAME_SHARED_HANDLER->getGameInfo(*(GAME_SHARED_HANDLER->levelInfoDict),GAME_SHARED_HANDLER->getCurLevel());
		Scene *gameLayer = Play::createScene();
		Director::sharedDirector()->replaceScene(TransitionCrossFade::create(0.3,gameLayer));
	}
}
void EndGameLayer::onExit()
{
	//TODO
	// Director::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	Layer::onExit();
}


