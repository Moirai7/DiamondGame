
#ifndef __llgame__ParseUtils__
#define __llgame__ParseUtils__

#include "Utils/json.h"
#include "Utils/value.h"
#include "Utils/reader.h"
#include "cocos2d.h"
#include "MonsterSprite.h"
using namespace cocos2d;
using namespace std;


class ParseUtils
{	
public:
	static ParseUtils* getInstance(){
		if(pu == NULL){
			pu = new ParseUtils();
			return pu;
		}else
			return pu;
	}
	void getGameInfo(const Json::Value& obj);
	void getGameInfo(const Json::Value& obj,int level);
	Json::Value getJsonFromFile(const char* fileName);
	int getMaxScore(){return maxscore;}
	int getCurLevel(){return curlevel;}
	int* getGameArray(){return game;}
	list<MonsterSprite*> getMonsters(){return monsters;}
private:
	ParseUtils(){}
	~ParseUtils(){
		if(pu!=NULL)  
		{  
			delete pu;  
		}  
	}
	static ParseUtils* pu;
	int maxscore;
	int game[49];
	list<MonsterSprite*> monsters;
	int curlevel;
};

#endif
