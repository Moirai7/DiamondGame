
#ifndef __LLgame__EndGameLayer__
#define __LLgame__EndGameLayer__


#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

class EndGameLayer: public CCLayer
{
private:
    
    Sprite* background;
    void setupGUI();
    
public:
    
   
    Size winSize;
    virtual bool init();
    void restartGame(Ref* r);
    void quitGame(Ref* r);
	void nextGame(Ref* r);
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::Scene* scene();
    
    virtual void onEnter();
    virtual void onExit();
    
    //function
       
    
    // implement the "static node()" method manually
    CREATE_FUNC( EndGameLayer );
};

#endif /* defined(__LL__EndGameLayer__) */
