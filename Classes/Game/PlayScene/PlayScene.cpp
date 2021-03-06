//
//  Play.cpp
//  
// 1、判断消除的是那种颜色，让宠物前进
// 2、判断宠物是否能攻击怪兽了
// 3、宝石被消除后也会攻击怪兽，3消4消5消等攻击的血量不同
// 4、记得宝石下落后再判定有没有可消除的
// 5、最最最最重要的：读取棋盘信息是从本地的一个文件读取的，什么文件，文件什么格式看你喜欢
//
//

#include "PlayScene.h"
#include "Game/SharedClass/GameConstants.h"

#define MATRIX_WIDTH (7)
#define MATRIX_HEIGHT (7)

#define diamond_GAP (1)
Play::Play()
	: spriteSheet(NULL)
	, m_matrix(NULL)
	, m_width(0)
	, m_height(0)
	, m_matrixLeftBottomX(0)
	, m_matrixLeftBottomY(0)
	, m_isNeedFillVacancies(false)
	, m_isAnimationing(true)//start with drop animation
	, m_isTouchEnable(true)
	, m_srcdiamond(NULL)
	, m_destdiamond(NULL)
	, m_movingVertical(true)//drop animation is vertical
	,gIsDisplayingHint(false)
	,m_removed(true)
	,m_checkM(false)
	,gameover(false)
	,m_monster(false)
{

}

Play::~Play()
{
	if (m_matrix) {
		free(m_matrix);
	}
}

Scene *Play::createScene()
{
	auto scene = Scene::create();
	auto layer = Play::create();
	scene->addChild(layer);
	return scene;
}

bool Play::init()
{
	if (!Layer::init()) {
		return false;
	}
	//TODO	加上monster的sprite 和 pet 

	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Audio/GameFieldMusic.mp3",true);

	gameLevel_ = GAME_SHARED_HANDLER->getCurrentLevelNumber();
	movesAllowed_=  GAME_SHARED_HANDLER->getTotalMoves(gameLevel_);
	scoreTarget_ = GAME_SHARED_HANDLER->getScoreTarget();
	pet_ = GAME_SHARED_HANDLER->getPet();
	GAME_SHARED_HANDLER->score = 0;
	monster =  GAME_SHARED_HANDLER->getMonsters();
	//pet_ = 1;

	//movesAllowed_=1;
	// background
	winSize = Director::getInstance()->getWinSize();
	auto background = Sprite::create(StringUtils::format("Backgrounds/back_%d.jpg",pet_));
	background->setAnchorPoint(Vec2(0, 1));
	background->setPosition(Vec2(0, winSize.height));
	this->addChild(background,-1);

	MenuItemImage *pauseButton = MenuItemImage::create(BUTTON_PAUSE, BUTTON_PAUSE_PRESSED, CC_CALLBACK_1(Play::pasueButtonPressed,this));
	Menu *pauseMenu = Menu::create(pauseButton,NULL);
	this->addChild(pauseMenu);
	pauseMenu->setPosition(Vec2(0,0));
	pauseButton->setPosition(Vec2(pauseButton->getContentSize().width/2.0, winSize.height-pauseButton->getContentSize().height/1.65));

	if(!monster.empty()){
		Sprite* coingBg = Sprite::create("GameLayer/bgr_moves.png");
		coingBg->setAnchorPoint(Vec2(0,0.5));
		coingBg->setPosition(Vec2(5.0,coingBg->getContentSize().height/1.8) );
		this->addChild(coingBg,2400);

		movesLabel = LabelBMFont::create("", FileUtils::sharedFileUtils()->fullPathForFilename("fonts/font.fnt").c_str(), 35);
		movesLabel->setAlignment( kCCTextAlignmentCenter );
		movesLabel->setPosition(Vec2(coingBg->getContentSize().width/2,coingBg->getContentSize().height/1.9));
		movesLabel->setScale(0.6*0.9);

		coingBg->addChild(movesLabel,27);
		movesLabel->setString(String::createWithFormat("%i",movesAllowed_)->getCString());

		//
		Sprite* blood = Sprite::create("GameLayer/bgr_score.png");
		blood->setAnchorPoint(Vec2(0,0.5));
		blood->setPosition(Vec2(winSize.width - blood->getContentSize().width,blood->getContentSize().height/1.8) );
		this->addChild(blood,2400);

		bloodLabel = LabelBMFont::create("", FileUtils::sharedFileUtils()->fullPathForFilename("fonts/font.fnt").c_str(), 35);
		bloodLabel->setAlignment( kCCTextAlignmentCenter );
		bloodLabel->setPosition(Vec2(blood->getContentSize().width/2,blood->getContentSize().height/1.9));
		bloodLabel->setScale(0.6*0.9);

		blood->addChild(bloodLabel,27);
		bloodLabel->setString(String::createWithFormat("%i",monster.front()->blood)->getCString());
	}

	// init spriteSheet
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Diamond/Diamond.plist");
	spriteSheet = SpriteBatchNode::create("Diamond/Diamond.pvr.ccz");
	addChild(spriteSheet);

	//创建monster的sprite 和 pet
	monster.front()->monster->setAnchorPoint(Vec2(0, 1));
	monster.front()->monster->setPosition(Vec2(winSize.width/5*4, winSize.height/3*2));//显示位置坐标需要调整
	addChild(monster.front()->monster, 30);

	pet = PetSprite::createPetSprite(pet_);
	pet->retain();
	pet->pet->setAnchorPoint(Vec2(0, 1));
	pet->pet->setPosition(Vec2(winSize.width/5*4,winSize.height/3));
	addChild(pet->pet,30);

	// Yes, you can load this value from other module.
	m_width = MATRIX_WIDTH;
	m_height = MATRIX_HEIGHT;

	// init position value
	m_matrixLeftBottomX = (winSize.width - DiamondSprite::getContentWidth() * m_width - (m_width - 1) * diamond_GAP) / 2;
	m_matrixLeftBottomY = (winSize.height - DiamondSprite::getContentWidth() * m_height - (m_height - 1) * diamond_GAP) / 2;

	// init point array
	int arraySize = sizeof(DiamondSprite *) * m_width * m_height;
	m_matrix = (DiamondSprite **)malloc(arraySize);
	memset((void*)m_matrix, 0, arraySize);

	initMatrix();
	scheduleUpdate();

	// bind touch event
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(Play::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(Play::onTouchMoved, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	gLastMoveTime = nowInSeconds();
	return true;
}
void Play::pasueButtonPressed(Ref* sender)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("Audio/ButtonClick.mp3");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	PauseLayer *pauseLayer = PauseLayer::create();
	this->addChild(pauseLayer,20000);

}
DiamondSprite *Play::diamondOfPoint(Point *point)
{
	DiamondSprite *diamond = NULL;
	Rect rect = Rect(0, 0, 0, 0);

	for (int i = 0; i < m_height * m_width; i++) {
		diamond = m_matrix[i];
		if (diamond) {
			rect.origin.x = diamond->getPositionX() - (diamond->getContentSize().width / 2);
			rect.origin.y = diamond->getPositionY() - (diamond->getContentSize().height / 2);
			rect.size = diamond->getContentSize();
			if (rect.containsPoint(*point)) {
				return diamond;
			}
		}
	}

	return NULL;
}

bool Play::getCheckPoint(){
	m_srcdiamond=NULL;
	m_destdiamond=NULL;
	for (int col = 0; col < m_width; col++) {
		// from buttom to top
		for (int row = 0; row < m_height; row++) {
			m_srcdiamond = m_matrix[row * m_width + col]; 
			if(row<m_width-1)
			{
				m_destdiamond = m_matrix[(row+1) * m_width + col];
				if(checkSwap()){
					return true;
				}
				else{
					if(col<m_height-1)
					{
						m_destdiamond = m_matrix[row * m_width + col+1];
						if(checkSwap()){
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}


bool Play::getHintPoint(){
	m_srcdiamond=NULL;
	m_destdiamond=NULL;
	for (int col = 0; col < m_width; col++) {
		// from buttom to top
		for (int row = 0; row < m_height; row++) {
			m_srcdiamond = m_matrix[row * m_width + col]; 
			if(row<m_width-1)
			{
				m_destdiamond = m_matrix[(row+1) * m_width + col];
				if(hintSwap()){
					return true;
				}
				else{
					if(col<m_height-1)
					{
						m_destdiamond = m_matrix[row * m_width + col+1];
						if(hintSwap()){
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

void Play::displayHint(){      
	getHintPoint();  
}
bool Play::onTouchBegan(Touch *touch, Event *unused)
{
	m_srcdiamond = NULL;
	m_destdiamond = NULL;
	gIsDisplayingHint=false;
	double currentTime = nowInSeconds();
	gLastMoveTime = currentTime;
	if (m_isTouchEnable) {
		auto location = touch->getLocation();
		m_srcdiamond = diamondOfPoint(&location);
	}
	return m_isTouchEnable;
}

void Play::onTouchMoved(Touch *touch, Event *unused)
{
	if (!m_srcdiamond || !m_isTouchEnable) {
		return;
	}

	int row = m_srcdiamond->getRow();
	int col = m_srcdiamond->getCol();

	auto location = touch->getLocation();
	auto halfdiamondWidth = m_srcdiamond->getContentSize().width / 2;
	auto halfdiamondHeight = m_srcdiamond->getContentSize().height / 2;

	auto  upRect = Rect(m_srcdiamond->getPositionX() - halfdiamondWidth,
		m_srcdiamond->getPositionY() + halfdiamondHeight,
		m_srcdiamond->getContentSize().width,
		m_srcdiamond->getContentSize().height);

	if (upRect.containsPoint(location)) {
		row++;
		if (row < m_height) {
			m_destdiamond = m_matrix[row * m_width + col];
		}
		m_movingVertical = true;
		swapdiamond();
		return;
	}

	auto  downRect = Rect(m_srcdiamond->getPositionX() - halfdiamondWidth,
		m_srcdiamond->getPositionY() - (halfdiamondHeight * 3),
		m_srcdiamond->getContentSize().width,
		m_srcdiamond->getContentSize().height);

	if (downRect.containsPoint(location)) {
		row--;
		if (row >= 0) {
			m_destdiamond = m_matrix[row * m_width + col];
		}
		m_movingVertical = true;
		swapdiamond();
		return;
	}

	auto  leftRect = Rect(m_srcdiamond->getPositionX() - (halfdiamondWidth * 3),
		m_srcdiamond->getPositionY() - halfdiamondHeight,
		m_srcdiamond->getContentSize().width,
		m_srcdiamond->getContentSize().height);

	if (leftRect.containsPoint(location)) {
		col--;
		if (col >= 0) {
			m_destdiamond = m_matrix[row * m_width + col];
		}
		m_movingVertical = false;
		swapdiamond();
		return;
	}

	auto  rightRect = Rect(m_srcdiamond->getPositionX() + halfdiamondWidth,
		m_srcdiamond->getPositionY() - halfdiamondHeight,
		m_srcdiamond->getContentSize().width,
		m_srcdiamond->getContentSize().height);

	if (rightRect.containsPoint(location)) {
		col++;
		if (col < m_width) {
			m_destdiamond = m_matrix[row * m_width + col];
		}
		m_movingVertical = false;
		swapdiamond();
		return;
	}

	// not a useful movement
}
bool Play::checkSwap(){
	m_isAnimationing = true;
	if (!m_srcdiamond || !m_destdiamond) {
		m_movingVertical = true;
		return false;
	}
	Point posOfSrc =m_srcdiamond->getPosition();
	Point posOfDest = m_destdiamond->getPosition();
	//Point posOfSrc = Vec2(m_srcdiamond->getPositionX() - (m_srcdiamond->getContentSize().width / 2),m_srcdiamond->getPositionY() - (m_srcdiamond->getContentSize().height / 2));
	//Point posOfDest = Vec2(m_destdiamond->getPositionX() - (m_destdiamond->getContentSize().width / 2),m_destdiamond->getPositionY() - (m_destdiamond->getContentSize().height / 2));
	float time = 0.2;

	// 1.swap in matrix
	m_matrix[m_srcdiamond->getRow() * m_width + m_srcdiamond->getCol()] = m_destdiamond;
	m_matrix[m_destdiamond->getRow() * m_width + m_destdiamond->getCol()] = m_srcdiamond;
	int tmpRow = m_srcdiamond->getRow();
	int tmpCol = m_srcdiamond->getCol();
	m_srcdiamond->setRow(m_destdiamond->getRow());
	m_srcdiamond->setCol(m_destdiamond->getCol());
	m_destdiamond->setRow(tmpRow);
	m_destdiamond->setCol(tmpCol);

	// 2.check for remove able
	std::list<DiamondSprite *> colChainListOfFirst;
	getColChain(m_srcdiamond, colChainListOfFirst);

	std::list<DiamondSprite *> rowChainListOfFirst;
	getRowChain(m_srcdiamond, rowChainListOfFirst);

	std::list<DiamondSprite *> colChainListOfSecond;
	getColChain(m_destdiamond, colChainListOfSecond);

	std::list<DiamondSprite *> rowChainListOfSecond;
	getRowChain(m_destdiamond, rowChainListOfSecond);
	bool check = false;
	if (colChainListOfFirst.size() >= 3
		|| rowChainListOfFirst.size() >= 3
		|| colChainListOfSecond.size() >= 3
		|| rowChainListOfSecond.size() >= 3) {
			
			check = true;
	}
	m_matrix[m_srcdiamond->getRow() * m_width + m_srcdiamond->getCol()] = m_destdiamond;
	m_matrix[m_destdiamond->getRow() * m_width + m_destdiamond->getCol()] = m_srcdiamond;
	tmpRow = m_srcdiamond->getRow();
	tmpCol = m_srcdiamond->getCol();
	m_srcdiamond->setRow(m_destdiamond->getRow());
	m_srcdiamond->setCol(m_destdiamond->getCol());
	m_destdiamond->setRow(tmpRow);
	m_destdiamond->setCol(tmpCol);
	return check;
}
bool Play::hintSwap(){
	m_isAnimationing = true;
	if (!m_srcdiamond || !m_destdiamond) {
		m_movingVertical = true;
		return false;
	}
	Point posOfSrc =m_srcdiamond->getPosition();
	Point posOfDest = m_destdiamond->getPosition();
	//Point posOfSrc = Vec2(m_srcdiamond->getPositionX() - (m_srcdiamond->getContentSize().width / 2),m_srcdiamond->getPositionY() - (m_srcdiamond->getContentSize().height / 2));
	//Point posOfDest = Vec2(m_destdiamond->getPositionX() - (m_destdiamond->getContentSize().width / 2),m_destdiamond->getPositionY() - (m_destdiamond->getContentSize().height / 2));
	float time = 0.2;

	// 1.swap in matrix
	m_matrix[m_srcdiamond->getRow() * m_width + m_srcdiamond->getCol()] = m_destdiamond;
	m_matrix[m_destdiamond->getRow() * m_width + m_destdiamond->getCol()] = m_srcdiamond;
	int tmpRow = m_srcdiamond->getRow();
	int tmpCol = m_srcdiamond->getCol();
	m_srcdiamond->setRow(m_destdiamond->getRow());
	m_srcdiamond->setCol(m_destdiamond->getCol());
	m_destdiamond->setRow(tmpRow);
	m_destdiamond->setCol(tmpCol);

	// 2.check for remove able
	std::list<DiamondSprite *> colChainListOfFirst;
	getColChain(m_srcdiamond, colChainListOfFirst);

	std::list<DiamondSprite *> rowChainListOfFirst;
	getRowChain(m_srcdiamond, rowChainListOfFirst);

	std::list<DiamondSprite *> colChainListOfSecond;
	getColChain(m_destdiamond, colChainListOfSecond);

	std::list<DiamondSprite *> rowChainListOfSecond;
	getRowChain(m_destdiamond, rowChainListOfSecond);

	if (colChainListOfFirst.size() >= 3
		|| rowChainListOfFirst.size() >= 3
		|| colChainListOfSecond.size() >= 3
		|| rowChainListOfSecond.size() >= 3) {
			m_matrix[m_srcdiamond->getRow() * m_width + m_srcdiamond->getCol()] = m_destdiamond;
			m_matrix[m_destdiamond->getRow() * m_width + m_destdiamond->getCol()] = m_srcdiamond;
			tmpRow = m_srcdiamond->getRow();
			tmpCol = m_srcdiamond->getCol();
			m_srcdiamond->setRow(m_destdiamond->getRow());
			m_srcdiamond->setCol(m_destdiamond->getCol());
			m_destdiamond->setRow(tmpRow);
			m_destdiamond->setCol(tmpCol);

			m_srcdiamond->runAction(Sequence::create(
				MoveTo::create(time, posOfDest),
				MoveTo::create(time, posOfSrc),
				NULL));
			m_destdiamond->runAction(Sequence::create(
				MoveTo::create(time, posOfSrc),
				MoveTo::create(time, posOfDest),
				NULL));
			return true;
	}
	m_matrix[m_srcdiamond->getRow() * m_width + m_srcdiamond->getCol()] = m_destdiamond;
	m_matrix[m_destdiamond->getRow() * m_width + m_destdiamond->getCol()] = m_srcdiamond;
	tmpRow = m_srcdiamond->getRow();
	tmpCol = m_srcdiamond->getCol();
	m_srcdiamond->setRow(m_destdiamond->getRow());
	m_srcdiamond->setCol(m_destdiamond->getCol());
	m_destdiamond->setRow(tmpRow);
	m_destdiamond->setCol(tmpCol);
	return false;
}
void Play::swapdiamond()
{
	m_isAnimationing = true;
	m_isTouchEnable = false;
	if (!m_srcdiamond || !m_destdiamond) {
		m_movingVertical = true;
		return;
	}

	Point posOfSrc = m_srcdiamond->getPosition();
	Point posOfDest = m_destdiamond->getPosition();
	float time = 0.2;

	// 1.swap in matrix
	m_matrix[m_srcdiamond->getRow() * m_width + m_srcdiamond->getCol()] = m_destdiamond;
	m_matrix[m_destdiamond->getRow() * m_width + m_destdiamond->getCol()] = m_srcdiamond;
	int tmpRow = m_srcdiamond->getRow();
	int tmpCol = m_srcdiamond->getCol();
	m_srcdiamond->setRow(m_destdiamond->getRow());
	m_srcdiamond->setCol(m_destdiamond->getCol());
	m_destdiamond->setRow(tmpRow);
	m_destdiamond->setCol(tmpCol);

	// 2.check for remove able
	std::list<DiamondSprite *> colChainListOfFirst;
	getColChain(m_srcdiamond, colChainListOfFirst);

	std::list<DiamondSprite *> rowChainListOfFirst;
	getRowChain(m_srcdiamond, rowChainListOfFirst);

	std::list<DiamondSprite *> colChainListOfSecond;
	getColChain(m_destdiamond, colChainListOfSecond);

	std::list<DiamondSprite *> rowChainListOfSecond;
	getRowChain(m_destdiamond, rowChainListOfSecond);

	if (colChainListOfFirst.size() >= 3
		|| rowChainListOfFirst.size() >= 3
		|| colChainListOfSecond.size() >= 3
		|| rowChainListOfSecond.size() >= 3) {
			movesAllowed_--;
			movesLabel->setString(String::createWithFormat("%i",movesAllowed_)->getCString());
			// just swap
			m_srcdiamond->runAction(MoveTo::create(time, posOfDest));
			m_destdiamond->runAction(MoveTo::create(time, posOfSrc));
			
			return;
	}

	// 3.no chain, swap back
	m_matrix[m_srcdiamond->getRow() * m_width + m_srcdiamond->getCol()] = m_destdiamond;
	m_matrix[m_destdiamond->getRow() * m_width + m_destdiamond->getCol()] = m_srcdiamond;
	tmpRow = m_srcdiamond->getRow();
	tmpCol = m_srcdiamond->getCol();
	m_srcdiamond->setRow(m_destdiamond->getRow());
	m_srcdiamond->setCol(m_destdiamond->getCol());
	m_destdiamond->setRow(tmpRow);
	m_destdiamond->setCol(tmpCol);

	m_srcdiamond->runAction(Sequence::create(
		MoveTo::create(time, posOfDest),
		MoveTo::create(time, posOfSrc),
		NULL));
	m_destdiamond->runAction(Sequence::create(
		MoveTo::create(time, posOfSrc),
		MoveTo::create(time, posOfDest),
		NULL));
}

void Play::createGameOver()
{
	Scene *endGameScene = EndGameLayer::scene();
	Director::sharedDirector()->replaceScene(TransitionCrossFade::create(0.5,endGameScene));   
}

double Play::nowInSeconds()
{
	struct timeval now;
	gettimeofday(&now, NULL); 
	double retVal = (now.tv_sec + (double)(now.tv_usec / 1000000.0f));

	return retVal;

}

void Play::update(float dt)
{
	double currentTime = nowInSeconds();
	//monster = GAME_SHARED_HANDLER->getMonsters();

	// check if animationing
	if (m_isAnimationing) {
		// init with false
		m_isAnimationing = false;
		for (int i = 0; i < m_height * m_width; i++) {
			DiamondSprite *diamond = m_matrix[i];
			if (diamond && diamond->getNumberOfRunningActions() > 0) {
				m_isAnimationing = true;
				break;
			}
		}
	}

	// if diamond is moving, ignore use touch event
	m_isTouchEnable = !m_isAnimationing;

	if (!m_isAnimationing) {
		if (m_isNeedFillVacancies) {
			//爆炸效果结束后才掉落新寿司，增强打击感
			fillVacancies();
			m_isNeedFillVacancies = false;
		} else {
			m_removed = checkAndRemoveChain();
			if(!m_removed){			
				if(m_monster){
					//Sprite* ms = monster.front()->monster;
					monster.pop_front();
					//ms = monster.front()->monster;
					if (!monster.empty())
					{
						monster.front()->monster->setAnchorPoint(Vec2(0, 1));
						monster.front()->monster->setPosition(Vec2(winSize.width/5*4, winSize.height/3*2));//显示位置坐标需要调整
						addChild(monster.front()->monster, 30);
						bloodLabel->setString(String::createWithFormat("%i",monster.front()->blood)->getCString());
					} 
					else
					{
						gameover = true;
					}
					m_monster = false;
				}
				if (movesAllowed_ == 0)
				{
					GAME_SHARED_HANDLER->score = 0;
					createGameOver();
				}else if(gameover){
					GAME_SHARED_HANDLER->score = 1;
					createGameOver();
				}else
				{
					if(!m_checkM){
						if(!getCheckPoint()){
							removeAllDiamond();
						}
					}
					m_checkM = true;
					if (currentTime - gLastMoveTime > kDelayBeforeHint && (!gIsDisplayingHint))
					{
						gIsDisplayingHint = true;
						displayHint();
					}
				}
				// Check for game over
			}else{
				m_checkM = false;
			}
		}
	}

}

//横向chain检查
void Play::getColChain(DiamondSprite *diamond, std::list<DiamondSprite *> &chainList)
{
	chainList.push_back(diamond);// add first diamond

	int neighborCol = diamond->getCol() - 1;
	while (neighborCol >= 0) {
		DiamondSprite *neighbordiamond = m_matrix[diamond->getRow() * m_width + neighborCol];
		if (neighbordiamond
			&& (neighbordiamond->getImgIndex() == diamond->getImgIndex())
			&& !neighbordiamond->getIsNeedRemove()
			&& !neighbordiamond->getIgnoreCheck()) {
				chainList.push_back(neighbordiamond);
				neighborCol--;
		} else {
			break;
		}
	}

	neighborCol = diamond->getCol() + 1;
	while (neighborCol < m_width) {
		DiamondSprite *neighbordiamond = m_matrix[diamond->getRow() * m_width + neighborCol];
		if (neighbordiamond
			&& (neighbordiamond->getImgIndex() == diamond->getImgIndex())
			&& !neighbordiamond->getIsNeedRemove()
			&& !neighbordiamond->getIgnoreCheck()) {
				chainList.push_back(neighbordiamond);
				neighborCol++;
		} else {
			break;
		}
	}
}

void Play::getRowChain(DiamondSprite *diamond, std::list<DiamondSprite *> &chainList)
{
	chainList.push_back(diamond);// add first diamond

	int neighborRow = diamond->getRow() - 1;
	while (neighborRow >= 0) {
		DiamondSprite *neighbordiamond = m_matrix[neighborRow * m_width + diamond->getCol()];
		if (neighbordiamond
			&& (neighbordiamond->getImgIndex() == diamond->getImgIndex())
			&& !neighbordiamond->getIsNeedRemove()
			&& !neighbordiamond->getIgnoreCheck()) {
				chainList.push_back(neighbordiamond);
				neighborRow--;
		} else {
			break;
		}
	}

	neighborRow = diamond->getRow() + 1;
	while (neighborRow < m_height) {
		DiamondSprite *neighbordiamond = m_matrix[neighborRow * m_width + diamond->getCol()];
		if (neighbordiamond
			&& (neighbordiamond->getImgIndex() == diamond->getImgIndex())
			&& !neighbordiamond->getIsNeedRemove()
			&& !neighbordiamond->getIgnoreCheck()) {
				chainList.push_back(neighbordiamond);
				neighborRow++;
		} else {
			break;
		}
	}
}

void Play::fillVacancies()
{
	// reset moving direction flag
	m_movingVertical = true;
	m_isAnimationing = true;

	Size size = Director::getInstance()->getWinSize();
	int *colEmptyInfo = (int *)malloc(sizeof(int) * m_width);
	memset((void *)colEmptyInfo, 0, sizeof(int) * m_width);

	// 1. drop exist diamond
	DiamondSprite *diamond = NULL;
	for (int col = 0; col < m_width; col++) {
		int removeddiamondOfCol = 0;
		// from buttom to top
		for (int row = 0; row < m_height; row++) {
			diamond = m_matrix[row * m_width + col];
			if (NULL == diamond) {
				removeddiamondOfCol++;
			} else {
				if (removeddiamondOfCol > 0) {
					// evey item have its own drop distance
					int newRow = row - removeddiamondOfCol;
					// switch in matrix
					m_matrix[newRow * m_width + col] = diamond;
					m_matrix[row * m_width + col] = NULL;
					// move to new position
					Point startPosition = diamond->getPosition();
					Point endPosition = positionOfItem(newRow, col);
					float speed = (startPosition.y - endPosition.y) / size.height;
					diamond->stopAllActions();// must stop pre drop action
					diamond->runAction(CCMoveTo::create(speed, endPosition));
					// set the new row to item
					diamond->setRow(newRow);
				}
			}
		}

		// record empty info
		colEmptyInfo[col] = removeddiamondOfCol;
	}

	// 2. create new item and drop down.
	for (int col = 0; col < m_width; col++) {
		for (int row = m_height - colEmptyInfo[col]; row < m_height; row++) {
			createAndDropdiamond(row, col);
		}
	}

	free(colEmptyInfo);
}

bool Play::removediamond()
{
	// make sequence remove
	m_isAnimationing = true;
	bool removed = false;
	for (int i = 0; i < m_height * m_width; i++) {
		DiamondSprite *diamond = m_matrix[i];
		if (!diamond) {
			continue;
		}

		if (diamond->getIsNeedRemove()) {
			//GAME_SHARED_HANDLER->score += 10 * pet_;
			m_isNeedFillVacancies = true;//需要掉落
			// TODO: 检查类型，并播放一行消失的动画
			//判断diamond的颜色根据个数打monster，然后判断monster现在的血量是否》0，小于的话
			//就调用 createGameOver();
			if(!monster.empty())
			{
				monster.front()->blood -= 10 * pet_;
				bloodLabel->setString(String::createWithFormat("%i",monster.front()->blood)->getCString());
				if (diamond->getImgIndex() == (pet_-1)&&!gameover)
				{
					monster.front()->blood -= 20 * pet_;
					bloodLabel->setString(String::createWithFormat("%i",monster.front()->blood)->getCString());
				}
				if (monster.front()->blood <= 0)
				{
					explodeMonster(monster.front()->monster);
				}
			}
			if(diamond->getDisplayMode() == DISPLAY_MODE_HORIZONTAL)
			{
				explodeSpecialH(diamond->getPosition());
			}
			else if (diamond->getDisplayMode() == DISPLAY_MODE_VERTICAL)
			{
				explodeSpecialV(diamond->getPosition());
			}
			explodediamond(diamond);
			removed = true;
		}
	}
	return removed;
}

void Play::removeAllDiamond()
{
	// make sequence remove
	m_isAnimationing = true;
	for (int i = 0; i < m_height * m_width; i++) {
		DiamondSprite *diamond = m_matrix[i];
		if (!diamond) {
			continue;
		}

		if (!monster.empty()){
			monster.front()->blood -= 10 * pet_;
			bloodLabel->setString(String::createWithFormat("%i",monster.front()->blood)->getCString());
			if (monster.front()->blood <= 0)
			{
				explodeMonster(monster.front()->monster);
			}
		}
		m_isNeedFillVacancies = true;//需要掉落

		if(diamond->getDisplayMode() == DISPLAY_MODE_HORIZONTAL)
		{
			explodeSpecialH(diamond->getPosition());
		}
		else if (diamond->getDisplayMode() == DISPLAY_MODE_VERTICAL)
		{
			explodeSpecialV(diamond->getPosition());
		}
		explodediamond(diamond);
	}
}

void Play::explodeSpecialH(Point point)
{
	Size size = Director::getInstance()->getWinSize();
	float scaleX = 4 ;
	float scaleY = 0.7 ;
	float time = 0.3;
	Point startPosition = point;
	float speed = 0.6f;

	auto colorSpriteRight = Sprite::create("Diamond/colorHRight.png");
	addChild(colorSpriteRight, 10);
	Point endPosition1 = Point(point.x - size.width, point.y);
	colorSpriteRight->setPosition(startPosition);
	colorSpriteRight->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
		MoveTo::create(speed, endPosition1),
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteRight)),
		NULL));

	auto colorSpriteLeft = Sprite::create("Diamond/colorHLeft.png");
	addChild(colorSpriteLeft, 10);
	Point endPosition2 = Point(point.x + size.width, point.y);
	colorSpriteLeft->setPosition(startPosition);
	colorSpriteLeft->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
		MoveTo::create(speed, endPosition2),
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteLeft)),
		NULL));


}

void Play::explodeSpecialV(Point point)
{
	Size size = Director::getInstance()->getWinSize();
	float scaleY = 4 ;
	float scaleX = 0.7 ;
	float time = 0.3;
	Point startPosition = point;
	float speed = 0.6f;

	auto colorSpriteDown = Sprite::create("Diamond/colorVDown.png");
	addChild(colorSpriteDown, 10);
	Point endPosition1 = Point(point.x , point.y - size.height);
	colorSpriteDown->setPosition(startPosition);
	colorSpriteDown->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
		MoveTo::create(speed, endPosition1),
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteDown)),
		NULL));

	auto colorSpriteUp = Sprite::create("Diamond/colorVUp.png");
	addChild(colorSpriteUp, 10);
	Point endPosition2 = Point(point.x , point.y + size.height);
	colorSpriteUp->setPosition(startPosition);
	colorSpriteUp->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
		MoveTo::create(speed, endPosition2),
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteUp)),
		NULL));
}

void Play::actionEndCallback(Node *node)
{
	DiamondSprite *diamond = (DiamondSprite *)node;
	m_matrix[diamond->getRow() * m_width + diamond->getCol()] = NULL;
	diamond->removeFromParent();
}

void Play::explodediamond(DiamondSprite *diamond)
{
	float time = 0.3;

	// 1. action for diamond
	diamond->runAction(Sequence::create(
		ScaleTo::create(time, 0.0),
		CallFuncN::create(CC_CALLBACK_1(Play::actionEndCallback, this)),
		NULL));

	// 2. action for circle
	auto circleSprite = Sprite::create("Diamond/circle.png");
	addChild(circleSprite, 10);
	circleSprite->setPosition(diamond->getPosition());
	circleSprite->setScale(0);// start size
	circleSprite->runAction(Sequence::create(ScaleTo::create(time, 1.0),
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, circleSprite)),
		NULL));

	// 3. particle effect
	auto particleStars = ParticleSystemQuad::create("Diamond/stars.plist");
	particleStars->setAutoRemoveOnFinish(true);
	particleStars->setBlendAdditive(false);
	particleStars->setPosition(diamond->getPosition());
	particleStars->setScale(0.3);
	addChild(particleStars, 20);
}

void Play::MactionEndCallback(Node *node)
{
	Sprite *diamond = (Sprite *)node;
	diamond->removeFromParent();
	m_monster = true;
}

void Play::explodeMonster(Sprite *diamond)
{
	float time = 0.3;

	// 1. action for diamond
	diamond->runAction(Sequence::create(
		ScaleTo::create(time, 0.0),
		CallFuncN::create(CC_CALLBACK_1(Play::MactionEndCallback, this)),
		NULL));

	auto jump = JumpBy::create(0.5, Vec2(0, 0), 100, 1);
	pet->pet->runAction(Sequence::create(jump, NULL, NULL));

	// 2. action for circle
	auto circleSprite = Sprite::create("Diamond/circle.png");
	addChild(circleSprite, 10);
	circleSprite->setPosition(diamond->getPosition());
	circleSprite->setScale(0);// start size
	circleSprite->runAction(Sequence::create(ScaleTo::create(time, 1.0),
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, circleSprite)),
		NULL));

	// 3. particle effect
	auto particleStars = ParticleSystemQuad::create("Diamond/stars.plist");
	particleStars->setAutoRemoveOnFinish(true);
	particleStars->setBlendAdditive(false);
	particleStars->setPosition(diamond->getPosition());
	particleStars->setScale(0.3);
	addChild(particleStars, 20);
}

bool Play::checkAndRemoveChain()
{
	DiamondSprite *diamond;
	// 1. reset ingnore flag
	for (int i = 0; i < m_height * m_width; i++) {
		diamond = m_matrix[i];
		if (!diamond) {
			continue;
		}
		diamond->setIgnoreCheck(false);
	}

	// 2. check chain
	for (int i = 0; i < m_height * m_width; i++) {
		diamond = m_matrix[i];
		if (!diamond) {
			continue;
		}

		if (diamond->getIsNeedRemove()) {
			continue;// 已标记过的跳过检查
		}
		if (diamond->getIgnoreCheck()) {
			continue;// 新变化的特殊寿司，不消除
		}

		// start count chain
		std::list<DiamondSprite *> colChainList;
		getColChain(diamond, colChainList);

		std::list<DiamondSprite *> rowChainList;
		getRowChain(diamond, rowChainList);

		std::list<DiamondSprite *> &longerList = colChainList.size() > rowChainList.size() ? colChainList : rowChainList;
		if (longerList.size() < 3) {
			continue;// 小于3个不消除
		}

		std::list<DiamondSprite *>::iterator itList;
		bool isSetedIgnoreCheck = false;
		for (itList = longerList.begin(); itList != longerList.end(); itList++) {
			diamond = (DiamondSprite *)*itList;
			if (!diamond) {
				continue;
			}

			if (longerList.size() > 3) {
				// 4消产生特殊寿司
				if (diamond == m_srcdiamond || diamond == m_destdiamond) {
					isSetedIgnoreCheck = true;
					diamond->setIgnoreCheck(true);
					diamond->setIsNeedRemove(false);
					diamond->setDisplayMode(m_movingVertical ? DISPLAY_MODE_VERTICAL : DISPLAY_MODE_HORIZONTAL);
					continue;
				}
			}

			markRemove(diamond);
		}

		// 如何是自由掉落产生的4消, 取最后一个变化为特殊寿司
		if (!isSetedIgnoreCheck && longerList.size() > 3) {
			diamond->setIgnoreCheck(true);
			diamond->setIsNeedRemove(false);
			diamond->setDisplayMode(m_movingVertical ? DISPLAY_MODE_VERTICAL : DISPLAY_MODE_HORIZONTAL);
		}
	}

	// 3.消除标记了的寿司
	return removediamond();
}

void Play::markRemove(DiamondSprite *diamond)
{
	if (diamond->getIsNeedRemove()) {
		return;
	}
	if (diamond->getIgnoreCheck()) {
		return;
	}

	// mark self
	diamond->setIsNeedRemove(true);
	// check for type and mark for certical neighbour
	if (diamond->getDisplayMode() == DISPLAY_MODE_VERTICAL) {
		for (int row = 0; row < m_height; row++) {
			DiamondSprite *tmp = m_matrix[row * m_width + diamond->getCol()];
			if (!tmp || tmp == diamond) {
				continue;
			}

			if (tmp->getDisplayMode() == DISPLAY_MODE_NORMAL) {
				tmp->setIsNeedRemove(true);
			} else {
				markRemove(tmp);
			}
		}
		// check for type and mark for horizontal neighbour
	} else if (diamond->getDisplayMode() == DISPLAY_MODE_HORIZONTAL) {
		for (int col = 0; col < m_width; col++) {
			DiamondSprite *tmp = m_matrix[diamond->getRow() * m_width + col];
			if (!tmp || tmp == diamond) {
				continue;
			}

			if (tmp->getDisplayMode() == DISPLAY_MODE_NORMAL) {
				tmp->setIsNeedRemove(true);
			} else {
				markRemove(tmp);
			}
		}
	}
}

void Play::initMatrix()
{
	int* arr = GAME_SHARED_HANDLER->getGameArray();
	for (int row = 0; row < m_height; row++) {
		for (int col = 0; col < m_width; col++) {
			if(arr[row*m_height+m_width]!=-1)
				createAndDropdiamond(row, col);
		}
	}
}

void Play::createAndDropdiamond(int row, int col)
{
	DiamondSprite *diamond = DiamondSprite::create(row, col);

	// create animation
	Point endPosition = positionOfItem(row, col);
	Point startPosition = Point(endPosition.x, endPosition.y + winSize.height / 2);
	diamond->setPosition(startPosition);
	float speed = startPosition.y / (1.5 * winSize.height);
	diamond->runAction(MoveTo::create(speed, endPosition));
	// add to BatchNode
	spriteSheet->addChild(diamond);

	m_matrix[row * m_width + col] = diamond;
}

Point Play::positionOfItem(int row, int col)
{
	float x = m_matrixLeftBottomX + (DiamondSprite::getContentWidth() + diamond_GAP) * col + DiamondSprite::getContentWidth() / 2;
	float y = m_matrixLeftBottomY + (DiamondSprite::getContentWidth() + diamond_GAP) * row + DiamondSprite::getContentWidth() / 2;
	return Point(x, y);
}