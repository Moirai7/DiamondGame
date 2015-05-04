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

CCScene* EndGameLayer::scene()
{
	CCScene *scene = CCScene::create();
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
	winSize = Director::sharedDirector()->getWinSize();
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
	background = Sprite::create(ENDGAME_BACKGROUND);
	background->setPosition( Vec2(winSize.width/2, winSize.height/2) );
	this->addChild(background);

	MenuItemImage *restartMenuItem = MenuItemImage::create(BUTTON_RESTART_GAME, BUTTON_RESTART_GAME_PRESSED,CC_CALLBACK_1(EndGameLayer::restartGame,this) );
	MenuItemImage *quitMenuItem = MenuItemImage::create(BUTTON_QUIT_GAME, BUTTON_QUIT_GAME_PRESSED, CC_CALLBACK_1(EndGameLayer::quitGame,this) );
	MenuItemImage *nextMenuItem = MenuItemImage::create(BUTTON_RESTART_GAME, BUTTON_RESTART_GAME_PRESSED,CC_CALLBACK_1(EndGameLayer::nextGame,this) );
	Menu *menu = Menu::create(restartMenuItem,quitMenuItem,nextMenuItem,NULL);
	menu->setPosition(Vec2(0,0));
	background->addChild(menu);

	restartMenuItem->setPosition(Vec2(winSize.width/2 - 150, winSize.height/2 - 250 ));
	restartMenuItem->setScale(0.65);

	nextMenuItem->setPosition(Vec2(winSize.width/2 , winSize.height/2 ));
	nextMenuItem->setScale(0.65);

	quitMenuItem->setPosition(Vec2(winSize.width/2 + 150, winSize.height/2 - 250 ));
	quitMenuItem->setScale(0.65);
	GameState *gs = GameState::sharedGameState();
	if(gs->score >= gs->getScoreTarget( gs->getCurrentLevelNumber() )&&GAME_SHARED_HANDLER->getCurLevel()<GAME_SHARED_HANDLER->getTotalLevel()){
		Sprite *sprite = Sprite::create(LEVELCOMPELTE_LABEL);
		sprite->setPosition(Vec2(winSize.width/2, winSize.height/2 + 320));
		this->addChild(sprite, 26);
		LabelBMFont* levelComlete = LabelBMFont::create("LEVEL COMPLETE",FileUtils::sharedFileUtils()->fullPathForFilename("font.fnt").c_str(), 35);
		levelComlete->setAlignment( kCCTextAlignmentCenter );
		levelComlete->setAnchorPoint( Vec2(0.5,0.5) );
		levelComlete->setWidth(winSize.width);
		levelComlete->setPosition(Vec2(winSize.width/2, winSize.height/2 + 360));
		levelComlete->setScale(0.8);
		this->addChild(levelComlete,26);


		LabelBMFont* score = LabelBMFont::create("Score: ", FileUtils::sharedFileUtils()->fullPathForFilename("font.fnt").c_str(), 35);
		score->setAlignment( kCCTextAlignmentCenter );
		score->setAnchorPoint( Vec2(0.5,0.5) );
		score->setPosition(Vec2(winSize.width/2, winSize.height/2 + 100));

		char buffer[64];
		sprintf(buffer, "Score: %d", GameState::sharedGameState()->score);
		score->setString(buffer);

		this->addChild(score, 26);

		//Menu *menu = Menu::create(restartMenuItem,quitMenuItem,nextMenuItem,NULL);
		//menu->setPosition(Vec2(0,0));
		//background->addChild(menu);

		//restartMenuItem->setPosition(Vec2(winSize.width/2 - 150, winSize.height/2 - 250 ));
		//restartMenuItem->setScale(0.65);

		//nextMenuItem->setPosition(Vec2(winSize.width/2 , winSize.height/2 ));
		//nextMenuItem->setScale(0.65);

		//quitMenuItem->setPosition(Vec2(winSize.width/2 + 150, winSize.height/2 - 250 ));
		//quitMenuItem->setScale(0.65);

	}
	else if(GAME_SHARED_HANDLER->getCurLevel()>=GAME_SHARED_HANDLER->getTotalLevel()&&gs->score >= gs->getScoreTarget( gs->getCurrentLevelNumber() )){
	//TODO
	}else{
		Sprite *sprite = Sprite::create(GAMEOVER_LABEL);
		sprite->setPosition(Vec2(winSize.width/2, winSize.height/2 + 160));
		this->addChild(sprite, 26);

		LabelBMFont* levelComlete = LabelBMFont::create("GAME OVER",FileUtils::sharedFileUtils()->fullPathForFilename("fonts/font.fnt").c_str(), 35);
		levelComlete->setAlignment( kCCTextAlignmentCenter );
		levelComlete->setAnchorPoint( Vec2(0.5,0.5) );
		levelComlete->setWidth(winSize.width);
		levelComlete->setPosition(Vec2(winSize.width/2, winSize.height/2 + 360));
		levelComlete->setScale(1.0);
		this->addChild(levelComlete,26);

		/*Menu *menu = Menu::create(restartMenuItem,quitMenuItem,NULL);
		menu->setPosition(Vec2(0,0));
		background->addChild(menu);

		restartMenuItem->setPosition(Vec2(winSize.width/2 - 150, winSize.height/2 - 250 ));
		restartMenuItem->setScale(0.65);

		quitMenuItem->setPosition(Vec2(winSize.width/2 + 150, winSize.height/2 - 250 ));
		quitMenuItem->setScale(0.65);*/

	}
}

void EndGameLayer::restartGame(Ref* r){

	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("Audio/ButtonClick.mp3");
	//CCScene *scene = CCScene::create();
	Scene *gameLayer = Play::createScene();
	//gameLayer->GameStart();
	// scene->addChild(gameLayer);

	Director::sharedDirector()->replaceScene(TransitionCrossFade::create(0.3,gameLayer));
}
void EndGameLayer::quitGame(Ref* r){

	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("Audio/ButtonClick.mp3");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Audio/MainMenuMusic.mp3");
	this->removeFromParent();
	CCScene *scene = CCScene::create();
	GameStart *playLayer = GameStart::create();
	scene->addChild(playLayer);
	Director::sharedDirector()->replaceScene(scene);
}
void EndGameLayer::nextGame(Ref* r){

	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("Audio/ButtonClick.mp3");
	//CCScene *scene = CCScene::create();
	Scene *gameLayer = Play::createScene();
	//gameLayer->GameStart();
	// scene->addChild(gameLayer);
	//GAME_SHARED_HANDLER->getGameInfo(GAME_SHARED_HANDLER->levelInfoDict,GAME_SHARED_HANDLER->getCurLevel()+1);
	if(GAME_SHARED_HANDLER->getCurLevel()<GAME_SHARED_HANDLER->getTotalLevel()){
		GAME_SHARED_HANDLER->setCurLevel(GAME_SHARED_HANDLER->getCurLevel()+1);
		GAME_SHARED_HANDLER->getGameInfo(*(GAME_SHARED_HANDLER->levelInfoDict),GAME_SHARED_HANDLER->getCurLevel());
		Director::sharedDirector()->replaceScene(TransitionCrossFade::create(0.3,gameLayer));
	}
}
void EndGameLayer::onExit()
{
	//TODO
	// Director::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	Layer::onExit();
}


