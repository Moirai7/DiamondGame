/****************************************************************************
 
 Match 3 Source Code
 
 Copyright (c) 2014 GameAcademy.com
 
 www.GameAcademy.com
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 ****************************************************************************/

#include "PauseLayer.h"
#include "Game/SharedClass/GameConstants.h"
#include "Game/GameStartScene/GameStartScene.h"
#include "SimpleAudioEngine.h"
using namespace cocos2d;

#define PAUSE_BACKGROUND "SocialUI/bgr_caption.png"

CCScene* PauseLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
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
    winSize = CCDirector::sharedDirector()->getWinSize();//TARGET_DESIGN_RESOLUTION_SIZE;
    setupGUI();
    this->setTouchEnabled(true);
    
    return true;
}

void PauseLayer:: onEnter()
{
	//TODO
 //   CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
    CCLayer::onEnter();
}

void PauseLayer::setupGUI()
{
    
    //add bg.
    background = CCSprite::create(PAUSE_BACKGROUND);
    background->setPosition( ccp(winSize.width/2, winSize.height/2) );
    this->addChild(background);
    
    MenuItemImage *resumeMenuItem = MenuItemImage::create(BUTTON_START_GAME, BUTTON_START_GAME_PRESSED, CC_CALLBACK_1(PauseLayer::resumeGame,this));
    MenuItemImage *quitMenuItem = MenuItemImage::create(BUTTON_QUIT_GAME, BUTTON_QUIT_GAME_PRESSED, CC_CALLBACK_1(PauseLayer::quitGame,this));
    
    Menu *menu = Menu::create(resumeMenuItem,quitMenuItem,NULL);
    menu->setPosition(Vec2(0,0));
    background->addChild(menu);
    
    resumeMenuItem->setPosition(ccp(winSize.width/2, winSize.height/2 + (250)));
    quitMenuItem->setPosition(ccp(winSize.width/2, winSize.height/2 ));
    
//    quitLabel->setString("QuitGame");

    
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
    CCScene *scene = CCScene::create();
    GameStart *playLayer = GameStart::create();
    scene->addChild(playLayer);
    CCDirector::sharedDirector()->replaceScene(scene);
}
void PauseLayer::onExit()
{
	//TODO
//    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
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
