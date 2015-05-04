//
//  GameStartScene.cpp
//
//

#include "GameStartScene.h"
#include "Game/SharedClass/GameConstants.h"
GameStart::GameStart(){
	// preload background music and effect
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic( "Audio/MainMenuMusic.mp3" );
	SimpleAudioEngine::getInstance()->preloadEffect( "Audio/ButtonClick.mp3" );

	// set default volume
	SimpleAudioEngine::getInstance()->setEffectsVolume(0.5);
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.5);
}

Scene * GameStart::createScene(){
	auto scene=Scene::create();
	auto layer=GameStart::create();
	scene->addChild(layer);
	return scene;
}
bool GameStart::init(){
	if(!Layer::init())
	{
		return false;
	}
	//Layer* layer = Layer::create();
	//auto rootNode =  CSLoader::createNode("MainScene.csb");//传入Studio2.x的资源路径 
	//layer->addChild(rootNode);//假设this是即将显示的scene 
	//this->addChild(layer);/*
	//button_start = dynamic_cast<Button*>(rootNode->getChildByTag(39));  
	//button_left = dynamic_cast<Button*>(rootNode->getChildByTag(40));  
	//button_right = dynamic_cast<Button*>(rootNode->getChildByTag(41));  */
	//button_start = dynamic_cast<Button*>(rootNode->getChildByName("Button_start"));  
	//button_left = dynamic_cast<Button*>(rootNode->getChildByName("Button_left"));  
	//button_right = dynamic_cast<Button*>(rootNode->getChildByName("Button_right")); 
	//button_start->setPressedActionEnabled(true);
	//button_left->setPressedActionEnabled(true);
	//button_right->setPressedActionEnabled(true);
	//button_start->addTouchEventListener(CC_CALLBACK_1(GameStart::startCallback, this));
	//button_left->addTouchEventListener(CC_CALLBACK_1(GameStart::leftCallback, this));
	//button_right->addTouchEventListener(CC_CALLBACK_1(GameStart::rightCallback, this));

	// Create the page view
	Size widgetSize = this->getContentSize();
	pageView = PageView::create();
	pageView->setContentSize(Size(widgetSize.width, widgetSize.height));
	pageView->setAnchorPoint(Vec2(0,0));
	pageView->setPosition(Vec2(0 ,0));

	int pageCount = 6;
	for (int i = 0; i < pageCount; ++i)
	{
		HBox* outerBox = HBox::create();
		outerBox->setContentSize(Size(widgetSize.width, widgetSize.height));
		outerBox->setAnchorPoint(Vec2(0,0));
		outerBox->setPosition(Vec2(0 ,0));
		outerBox->setBackGroundImage(StringUtils::format("Backgrounds/back_%d.jpg",i+1));

		Sprite* sprite = Sprite::create(StringUtils::format("LevelSelection/thumb%d.png",i+1));
		//Sprite* sprite = Sprite::create(StringUtils::format("Monster/Monster%d.png",i+1));
		sprite->setPosition(Vec2(widgetSize.width/2, widgetSize.height/2));
		outerBox->addChild(sprite);

		auto actionUp = JumpBy::create(2, Vec2(0,0), 80, 4);
		RepeatForever* act = RepeatForever::create(actionUp);
		sprite->runAction(act);

		pageView->insertPage(outerBox,i);
	}

	//pageView->addEventListener(CC_CALLBACK_2(GameStart::pageViewEvent, this));
	pageView->setName("pageView");
	this->addChild(pageView);

	//Button *btn_left = Button::create("LevelSelection/LS01.png","LevelSelection/LS02.png","LevelSelection/LS03.png");
	//btn_left->setPosition(Vec2(widgetSize.width/6, widgetSize.height/3));
	//this->addChild(btn_left);
	//btn_left->setName(StringUtils::format("btn_left"));
	//btn_left->setGlobalZOrder(1);
	//btn_left->addTouchEventListener( CC_CALLBACK_1(GameStart::leftCallback, this));

	//Button *btn_right = Button::create("LevelSelection/LS01.png","LevelSelection/LS02.png","LevelSelection/LS03.png");
	//auto a1 = RotateBy::create(0.0f, 180);
	//btn_right->runAction(Sequence::create(a1, nullptr));
	//btn_left->setGlobalZOrder(1);
	//btn_right->setPosition(Vec2(widgetSize.width-widgetSize.width/6, widgetSize.height/3));
	//this->addChild(btn_right);
	//btn_right->setName(StringUtils::format("btn_right"));
	//btn_right->addTouchEventListener( CC_CALLBACK_1(GameStart::rightCallback, this));

	MenuItemImage *playMenuItem = MenuItemImage::create("LevelSelection/LS07.png", "LevelSelection/LS08.png", CC_CALLBACK_1(GameStart::startCallback,this));
	Menu *menu = Menu::create(playMenuItem,NULL);   
	menu->setPosition(Vec2(0, 0));
	playMenuItem->setPosition(Vec2(widgetSize.width/2, widgetSize.height/3-50));

	this->addChild(menu,10);
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Audio/MainMenuMusic.mp3",true);
	return true;
}

void GameStart::pageViewEvent(Ref *pSender, PageView::EventType type)
{
	switch (type)
	{
	case PageView::EventType::TURNING:
		{
			//startCallback(NULL);
			PageView* pageView = dynamic_cast<PageView*>(pSender);
			log(CCString::createWithFormat("page = %ld", pageView->getCurPageIndex() + 1)->getCString());
		}
		break;

	default:
		break;
	}
}

void GameStart::startCallback(Ref* sender)
{
	GAME_SHARED_HANDLER->setPet(pageView->getCurPageIndex() + 1);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("Audio/ButtonClick.mp3");
	//Scene* scene = Scene::create();
	Scene *gameLayer = Play::createScene();
	GAME_SHARED_HANDLER->setCurLevel(0);
	GAME_SHARED_HANDLER->getGameInfo(*(GAME_SHARED_HANDLER->levelInfoDict),GAME_SHARED_HANDLER->getCurLevel());
   // gameLayer->GameStart();
    //scene->addChild(gameLayer);

	Director::getInstance()->replaceScene(TransitionCrossFade::create(0.5,gameLayer));

}
void GameStart:: onEnter()
{
  
  Layer::onEnter();
}

void GameStart:: onExit()
{
    Layer::onExit();
    
    NotificationCenter::sharedNotificationCenter()->removeAllObservers(this);

}

void GameStart::leftCallback(Ref* sender)
{
	log(CCString::createWithFormat("page = %ld left", pageView->getCurPageIndex() + 1)->getCString());
}


void GameStart::rightCallback(Ref* sender)
{
	log(CCString::createWithFormat("page = %ld right", pageView->getCurPageIndex() + 1)->getCString());
}