//
//  GameStartScene.h
//
//
//

#ifndef __LL__GameStartScene__
#define __LL__GameStartScene__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Game/PlayScene/PlayScene.h"
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace CocosDenshion;
using namespace cocostudio;
using namespace cocostudio::timeline;


class GameStart:public Layer {
public:
    CREATE_FUNC(GameStart);
    static Scene * createScene();
	GameStart();
	virtual void onEnter();
    virtual void onExit();
    bool init();
private:
	void startCallback(Ref* sender);
	void leftCallback(Ref* sender);
	void rightCallback(Ref* sender);
	PageView* pageView;
	Button* button_start;
	Button* button_left;
	Button* button_right;
	void playGame();

	void pageViewEvent(Ref *pSender, PageView::EventType type);
	//void onButtonClicked(Ref* sender, Widget::TouchEventType type);
};
#endif /* defined(__llgame__GameStartScene__) */
