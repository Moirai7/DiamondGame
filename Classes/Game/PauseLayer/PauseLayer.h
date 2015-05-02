

#ifndef __llgame__PauseLayer__
#define __llgame__PauseLayer__


#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

class PauseLayer: public CCLayer
{
private:
    CCSprite* background;
    void setupGUI();
public:
    
    bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void onTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    CCSize winSize;
    virtual bool init();
    void resumeGame(Ref* sender);
    void quitGame(Ref* sender);
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::Scene* scene();
    
    virtual void onEnter();
    virtual void onExit();
    
    //function
       
    
    // implement the "static node()" method manually
    CREATE_FUNC(PauseLayer);
};

#endif /* defined(__match3__PauseLayer__) */
