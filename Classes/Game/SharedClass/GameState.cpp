

#include <iostream>
#include "GameState.h"
#include "GameUtils.h"
#include "GameConstants.h"
#include "SimpleAudioEngine.h"


#define KEY_LIFE_COUNT "lifeCount"
#define KTAG_GAME_PLAYED_FIRST_TIME "GameCount"
#define K_TIME_TAG "initTime"


using namespace std;
using namespace cocos2d;
bool GameState::instanceFlag = false;
GameState* GameState::single = NULL;
GameState::GameState()
{
    
}

GameState* GameState::create()
{
    GameState * pRet = new GameState();
        
    if (pRet && pRet->init())
    {
        return pRet;
    }

    return NULL;
}



Json::Value* GameState::getJsonFromFile(const char* fileName){
	CCFileUtils* t_pFileUtils = CCFileUtils::sharedFileUtils();

	string t_oStrFullPath = t_pFileUtils->fullPathForFilename(fileName);

	unsigned char* t_pArrCharBuffer = NULL;
	ssize_t t_lBufferSize = 0;

	t_pArrCharBuffer = t_pFileUtils->getFileData(t_oStrFullPath.c_str(), "r", &t_lBufferSize);
	string t_oStrContent((char*)t_pArrCharBuffer, t_lBufferSize);
	Json::Value* t_oValueSprites = new Json::Value();
	Json::Reader t_oReader;

	if(!t_oReader.parse(t_oStrContent, *t_oValueSprites)) {
		CCASSERT(false, "Json::Reader Parse error!");
	}
	if (t_pArrCharBuffer) {
		delete [] t_pArrCharBuffer;
		t_pArrCharBuffer = NULL;
	}
	
	return t_oValueSprites;
}

void GameState::getGameInfo(const Json::Value& obj,int level){
	const Json::Value tmp_level = obj[level];
	maxscore = tmp_level["MaxScore"].asInt();
	Json::Value tmp_game= tmp_level["GameArray"];
	for(int k=0;k<49;k++){
		game[k]=tmp_game[k]["V"].asInt();
	}
	maxmove = tmp_level["MaxMove"].asInt();
	Json::Value tmp_monster = tmp_level["Monster"];		
	for(int j=0;j<tmp_monster.size();j++){
		int num = tmp_monster[j]["MonsterNum"].asInt();
		int blood = tmp_monster[j]["MonsterBlood"].asInt();
		MonsterSprite* ms = MonsterSprite::createMonsterSprite(num,blood);
		monsters.push_back(ms);
	}
}

void GameState::getGameInfo(const Json::Value& obj)
{
	curlevel = obj["CurLevel"].asInt();
	totalLevel = obj["TotalLevel"].asInt();
	*levelInfoDict = obj["Level"];
	getGameInfo(*levelInfoDict,curlevel);
}



bool GameState::init()
{
    if (Node::init())
    {
        isNetWorkAvailAble_ = false;
        UserDefault::sharedUserDefault()->flush();
        
        levelInfoDict = getJsonFromFile(LEVEL_INFO_PLIST); 
		getGameInfo(*levelInfoDict);
		levelNo =  getCurLevel();

        totalLife= UserDefault::sharedUserDefault()->getIntegerForKey(KEY_LIFE_COUNT, 1);
        //CCLog("log:: totalLive %d",totalLife);
        timeInSeconds= UserDefault::sharedUserDefault()->getDoubleForKey(K_TIME_TAG,0.0);
        
        
        
        int gameCount= UserDefault::sharedUserDefault()->getIntegerForKey(KTAG_GAME_PLAYED_FIRST_TIME, 0);
        if (gameCount==0){
            struct timeval now;
            gettimeofday(&now, NULL); 
            timeInSeconds = (now.tv_sec + (double)(now.tv_usec / 1000000.0f));
            UserDefault::sharedUserDefault()->setDoubleForKey(K_TIME_TAG,timeInSeconds);
            UserDefault::sharedUserDefault()->setIntegerForKey(KTAG_GAME_PLAYED_FIRST_TIME, 1);
            UserDefault::sharedUserDefault()->flush();
        }
        else
        {
            timerForLifeCheck(0.0f);
        }

        
        this->schedule(schedule_selector(GameState::timerForLifeCheck),1/60);

         return true;
    }
    
    return false;
   
}

int GameState::getScoreTarget(int levelNum)
{
	return maxscore;
}

int GameState::getTotalMoves(int levelNum)
{
	return maxmove;
}

void GameState::updateScoreToServer()
{
    if (isNetWorkAvailAble_ == false) {
        //CCLog("log:: isnetworkAvailAble %d",isNetWorkAvailAble_);
        return;
    }
    int totalScore = 0;
    for(int i=1 ; i<=TOTAL_LEVELS ; i++){
    
        const char *key = String::createWithFormat("%s%d",LEVEL_KEY_PREFIX,i)->getCString();
        int score = UserDefault::sharedUserDefault()->getIntegerForKey(key);
        if(score==0){
            break;
        }
        totalScore+=score;
        
    }

   //CCLog("log:: in update ScoreToServer");
//   CPP_FacebookGameController::getInstance()->FB_SendScore(totalScore);

}

int GameState::getCurrentLevelNumber(){
    return levelNo;
}
void GameState::setCurrentLevelNumber(int level){
    levelNo = level;
}
bool GameState::addLevelProgress(int score, int level){
    const char *key = String::createWithFormat("%s%d",LEVEL_KEY_PREFIX,level)->getCString();
    int perviousHighScore = UserDefault::sharedUserDefault()->getIntegerForKey(key);
    int targetScore = GAME_SHARED_HANDLER->getScoreTarget(level);

    
    if(score>targetScore){
             
    }
    
    else{
        GAME_SHARED_HANDLER->giveLife(1, false);
    }
    
    
    if (perviousHighScore < score) {
        UserDefault::sharedUserDefault()->setIntegerForKey(key, score);
        UserDefault::sharedUserDefault()->flush();
        return true;
    }
    return false;
}


int GameState::getCompletedLevelCount(){

    for (int i=0; i<TOTAL_LEVELS ; i++){
        if(!GAME_SHARED_HANDLER->isLevelCompleted(i+1)){
            return i;
        }
    }
    return TOTAL_LEVELS;
}


bool GameState::isLevelCompleted(int level){
    const char *key = String::createWithFormat("%s%d",LEVEL_KEY_PREFIX,level)->getCString();
    int highscore = UserDefault::sharedUserDefault()->getIntegerForKey(key);
    int targetScore = GAME_SHARED_HANDLER->getScoreTarget(level);

    if(highscore>targetScore){
        return true;
    }
    return false;
}
int GameState::getLevelStars(int level){
    
    const char *key = String::createWithFormat("%s%d",LEVEL_KEY_PREFIX,level)->getCString();
    int highscore = UserDefault::sharedUserDefault()->getIntegerForKey(key);
    int targetScore = GAME_SHARED_HANDLER->getScoreTarget(level);
    int stars = 0;
    
    float percentage =(highscore/targetScore)*75;
    
    if (percentage<75)
        stars=0;
    else if (percentage>74 && percentage<83)
        stars=1;
    else if (percentage>82 && percentage<93)
        stars=2;
    else
        stars=3;
    
    
    return stars;
}

int GameState::getLevelStarsWithScore(int level,int score){
    

    int highscore = score;
    int targetScore = GAME_SHARED_HANDLER->getScoreTarget(level);
    int stars = 0;
    
    float percentage =(highscore/targetScore)*75;
    
    if (percentage<75)
        stars=0;
    else if (percentage>74 && percentage<83)
        stars=1;
    else if (percentage>82 && percentage<93)
        stars=2;
    else
        stars=3;
    
    
    return stars;
}

int GameState::getLevelHighScore(int levelNum){
    String *key = String::createWithFormat("%s%d",LEVEL_KEY_PREFIX,levelNum);
    int levelHighScore = UserDefault::sharedUserDefault()->getIntegerForKey(key->getCString());
    return levelHighScore;
}

void GameState::timerForLifeCheck(float dt)
{
    struct timeval now;
    gettimeofday(&now, NULL);
    double retVal = (now.tv_sec + (double)(now.tv_usec / 1000000.0f));
    float diffInSec=retVal-timeInSeconds;
    int totalVal = diffInSec/Total_Life_Refresh_Time_In_sec;
    if (totalVal>0)
    {
        giveLife(totalVal, true);
        //// Again Refresh Counter of Timer
        struct timeval now;
        gettimeofday(&now, NULL);
        timeInSeconds = (now.tv_sec + (double)(now.tv_usec / 1000000.0f));
        UserDefault::sharedUserDefault()->setFloatForKey(K_TIME_TAG,timeInSeconds);
        UserDefault::sharedUserDefault()->flush();
    }

}

int GameState::getRemainingTimeForLife(){
    
    struct timeval now;
    gettimeofday(&now, NULL);
    double retVal = (now.tv_sec + (double)(now.tv_usec / 1000000.0f));
    float diffInSec=retVal-timeInSeconds;
    
    
    if(diffInSec>Total_Life_Refresh_Time_In_sec){
        return 0;
    }
    else{
        return Total_Life_Refresh_Time_In_sec-diffInSec;
    }
}




int GameState::getLives(){
    return totalLife;
}

void GameState::giveLife(int quantity, bool add)
{
    
	//CCLog("log:: quantity %d",quantity);
    if (add){
        totalLife+=quantity;
    	//CCLog("log:: totallife1 %d",totalLife);

    }
    else
    {
        totalLife=totalLife-quantity;
    	//CCLog("log:: totallife2 %d",totalLife);
    }
    if(totalLife>MAX_LIVES_COUNT){
        totalLife = MAX_LIVES_COUNT;
    }
	//CCLog("log:: totallife3 %d",totalLife);

    NotificationCenter::sharedNotificationCenter()->postNotification(N_LIFE_COUNT_UPDATED);
    
    UserDefault::sharedUserDefault()->setIntegerForKey(KEY_LIFE_COUNT, totalLife);
    UserDefault::sharedUserDefault()->flush();

}

GameState* GameState::sharedGameState()
{
    if(! instanceFlag)
    {
        single = GameState::create();//new GameState();
        instanceFlag = true;

        return single;
    }
    else
    {
        return single;
    }
}
