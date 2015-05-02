

#ifndef EggJump_GameState_h
#define EggJump_GameState_h
#include "cocos2d.h"
#include "Utils/json.h"
#include "Game/Models/MonsterSprite.h"
using namespace Json;
using namespace std;
class GameState:public cocos2d::CCNode
{
private:
    static bool instanceFlag;
    static GameState *single;
    
    int totalLife;
    int levelNo;
    double timeInSeconds;

    GameState();
    virtual bool init();
    static GameState * create(void);

	int maxscore;
	int game[49];
	int curlevel;
	int maxmove;
	list<MonsterSprite*> monsters;
	int pet;
	int totalLevel;

public:
    CC_SYNTHESIZE(bool, isNetWorkAvailAble_, isNetWorkAvailAble);
    static GameState* sharedGameState();
    int levelCompleted; // TODO: delete
    int score;  
    Json::Value* levelInfoDict; 
    int getLives();
    int getCurrentLevelNumber();
    void setCurrentLevelNumber(int level);
    void giveLife(int,bool);
    int getScoreTarget();
    int getTotalMoves(int levelNum);
    int getLevelHighScore(int levelNum);
    void updateLevelHighScore(int score, const char *key);
    int getLevelHighScore(const char* key);
    bool addLevelProgress(int score, int level);
    bool isLevelCompleted(int level);
    int getLevelStars(int level);
    int getLevelStarsWithScore(int level,int score);
    void updateScoreToServer();
    void timerForLifeCheck(float dt);
    int getRemainingTimeForLife();
    int getCompletedLevelCount();
	virtual ~GameState() {instanceFlag = false;   }

	Json::Value* getJsonFromFile(const char* fileName);

	void getGameInfo(const Json::Value& obj);
	void getGameInfo(const Json::Value& obj,int level);
	int getCurLevel(){return curlevel;}
	void setCurLevel(int c){curlevel = c;}
	int* getGameArray(){return game;}
	void setPet(int a){pet = a;}
	int getPet(){return pet;}
	int getTotalLevel(){return totalLevel;}

	list<MonsterSprite*> getMonsters(){return monsters;}
};
#endif
