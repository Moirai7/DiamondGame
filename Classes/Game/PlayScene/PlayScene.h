//
//  Play.h
// 1、判断消除的是那种颜色，让宠物前进
// 2、判断宠物是否能攻击怪兽了
// 3、宝石被消除后也会攻击怪兽，3消4消5消等攻击的血量不同
// 4、记得宝石下落后再判定有没有可消除的
// 5、最最最最重要的：读取棋盘信息是从本地的一个文件读取的，什么文件，文件什么格式看你喜欢
// 6、可添加消除的时候的效果，但不是重点
// 7、去试试那个游戏就明白了
//
//

#ifndef __llgame__PlayScene__
#define __llgame__PlayScene__

#include "cocos2d.h"
#include "Game\Models\DiamondSprite.h"
#include "Game\PauseLayer\PauseLayer.h"
#include "Game\EndGameLayer\EndGameLayer.h"
#include "SimpleAudioEngine.h"
#include "Game\Models\MonsterSprite.h"
#include "Game\Models\PetSprite.h"
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace CocosDenshion;
enum class GAMEDIR{
 DIR_UP,
    DIR_LEFT,
    DIR_RIGHT,
    DIR_DOWN
};
class Play:public Layer{
public:
    Play();
    ~Play();
    static Scene* createScene();
    CREATE_FUNC(Play);
    
    virtual bool init() override;
    virtual void update(float dt) override;
    virtual bool onTouchBegan(Touch *touch, Event *unused) override;
    virtual void onTouchMoved(Touch *touch, Event *unused) override;
private:
	int gameLevel_;
    int scoreTarget_;
    int movesAllowed_;
	int pet_;
	bool m_removed;
	bool m_monster;
	Size winSize;

    SpriteBatchNode *spriteSheet;
    DiamondSprite **m_matrix;// 2D array which store (DiamondSprite *).
    int m_width;
    int m_height;
    // for rapid count position
    float m_matrixLeftBottomX;
    float m_matrixLeftBottomY;
    bool m_isTouchEnable;// is need to deal with touch event
    DiamondSprite *m_srcdiamond;// 用户移动的寿司
    DiamondSprite *m_destdiamond;// 移动到哪个寿司
	PetSprite *pet;//该关卡的宠物
	list<MonsterSprite*> monster;//该关卡的怪物组
    bool m_isAnimationing;
	bool m_checkM;
    bool m_isNeedFillVacancies;//是否有空缺需要填补
    bool m_movingVertical;// true: 4消除产生纵向炸弹.  false: 4消除产生横向炸弹.
	bool gameover;
    
    void initMatrix();
    void createAndDropdiamond(int row, int col);
    Point positionOfItem(int row, int col);
    bool checkAndRemoveChain();
    void getColChain(DiamondSprite *diamond, std::list<DiamondSprite *> &chainList);
    void getRowChain(DiamondSprite *diamond, std::list<DiamondSprite *> &chainList);
    bool removediamond();
	void removeAllDiamond();
    void actionEndCallback(Node *node);
    void explodediamond(DiamondSprite *diamond);
	void MactionEndCallback(Node *node);
    void explodeMonster(Sprite *diamond);
    
    void explodeSpecialH(Point point);
    void explodeSpecialV(Point point);
    
    void fillVacancies();
    DiamondSprite *diamondOfPoint(Point *point);
    void swapdiamond();
    void markRemove(DiamondSprite *diamond);

	void pasueButtonPressed(Ref* sender);
	LabelBMFont* movesLabel;
	LabelBMFont* bloodLabel;
	void createGameOver();
    void displayHint();
	bool gIsDisplayingHint;
	bool getHintPoint();
	bool getCheckPoint();
	bool checkSwap();
	bool hintSwap();
	double nowInSeconds();
	double gLastMoveTime;
};
#endif /* defined(__llgame__Play__) */
