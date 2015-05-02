
#include "PauseLayer.h"
#include "Game/SharedClass/GameConstants.h"
#include "Game/GameStartScene/GameStartScene.h"
#include "SimpleAudioEngine.h"
using namespace cocos2d;

#define PAUSE_BACKGROUND "SocialUI/bgr_caption.png"

Scene* PauseLayer::scene()
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();
    
    // 'layer' is an autorelease object
    PauseLayer *layer = PauseLayer::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool PauseLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if (!cocos2d::CCLayer::init())
    {
        return false;
    }
    winSize = Director::sharedDirector()->getWinSize();//TARGET_DESIGN_RESOLUTION_SIZE;
    setupGUI();
    this->setTouchEnabled(true);
    
    return true;
}

void PauseLayer:: onEnter()
{
	//TODO
 //   Director::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
    CCLayer::onEnter();
}

void PauseLayer::setupGUI()
{
    
    //add bg.
    background = CCSprite::create(PAUSE_BACKGROUND);
    background->setPosition( Vec2(winSize.width/2, winSize.height/2) );
    this->addChild(background);
    
    MenuItemImage *resumeMenuItem = MenuItemImage::create(BUTTON_START_GAME, BUTTON_START_GAME_PRESSED, CC_CALLBACK_1(PauseLayer::resumeGame,this));
    MenuItemImage *quitMenuItem = MenuItemImage::create(BUTTON_QUIT_GAME, BUTTON_QUIT_GAME_PRESSED, CC_CALLBACK_1(PauseLayer::quitGame,this));
    
    Menu *menu = Menu::create(resumeMenuItem,quitMenuItem,NULL);
    menu->setPosition(Vec2(0,0));
    background->addChild(menu);
    
    resumeMenuItem->setPosition(Vec2(winSize.width/2, winSize.height/3 + (250)));
    quitMenuItem->setPosition(Vec2(winSize.width/2, winSize.height/3 ));
    
}

void PauseLayer::resumeGame(Ref* sender)
{
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("ButtonClick.mp3");
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("GameFieldMusic.mp3");
    this->removeFromParent();
}
void PauseLayer::quitGame(Ref* sender)
{
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("ButtonClick.mp3");
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("MainMenuMusic.mp3");
    this->removeFromParent();
    Scene *scene = Scene::create();
    GameStart *playLayer = GameStart::create();
    scene->addChild(playLayer);
    Director::sharedDirector()->replaceScene(scene);
}
void PauseLayer::onExit()
{
	//TODO
//    Director::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}


bool PauseLayer::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}
void PauseLayer::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void PauseLayer::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void PauseLayer::onTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
