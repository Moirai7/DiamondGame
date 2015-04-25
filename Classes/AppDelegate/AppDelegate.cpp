#include "AppDelegate.h"
#include "cocos2d.h"
#include "Game/SharedClass/GameConstants.h"
#include "Game/GameStartScene/GameStartScene.h"
#include "Game/EndGameLayer/EndGameLayer.h"
#include "Game/PlayScene/PlayScene.h"
#include "Game/SharedClass/GameState.h"
#include "SimpleAudioEngine.h"
#define DESIGN_WIDTH 640  
#define DESIGN_HEIGHT 960 

USING_NS_CC;
using namespace std;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
     // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("diamond");
        director->setOpenGLView(glview);
    }
   //glview->setFrameSize(DESIGN_WIDTH,DESIGN_HEIGHT);
  /* glview->setDesignResolutionSize(320.0f, 480.0f, ResolutionPolicy::FIXED_HEIGHT);
    std::vector<std::string> searchPath;
    searchPath.push_back("w640");
    CCFileUtils::getInstance()->setSearchPaths(searchPath);
    director->setContentScaleFactor(640.0f / 320.0f);*/

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = GameStart::createScene();
	//auto scene = EndGameLayer::scene();
    // run
    director->runWithScene(scene);
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    Director::sharedDirector()->pause();
    Director::sharedDirector()->stopAnimation();
    //CCLog("BackEnd");
    // if you use SimpleAudioEngine, it must be pause
    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    //    ccGLInvalidateStateCache();
    //    CCShaderCache::sharedShaderCache()->reloadDefaultShaders();
    //    ccDrawInit();
    //    CCTextureCache::reloadAllTextures();
    //    //CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_COME_TO_FOREGROUND, NULL);
    //    CCDirector::sharedDirector()->setGLDefaultValues();
    
    Director::sharedDirector()->resume();
    Director::sharedDirector()->startAnimation();
    
    //CCLog("ForeEnd");
    // if you use SimpleAudioEngine, it must resume here
   CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
