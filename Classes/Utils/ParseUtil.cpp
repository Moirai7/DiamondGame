
#include "ParseUtil.h"
#include <fstream>
using namespace std;
ParseUtils* ParseUtils::pu=NULL;//为静态变量m_Singleton分配空间  
Json::Value ParseUtils::getJsonFromFile(const char* fileName){
	CCFileUtils* t_pFileUtils = CCFileUtils::sharedFileUtils();

	string t_oStrFullPath = t_pFileUtils->fullPathForFilename(fileName);

	unsigned char* t_pArrCharBuffer = NULL;
	ssize_t t_lBufferSize = 0;

	t_pArrCharBuffer = t_pFileUtils->getFileData(t_oStrFullPath.c_str(), "r", &t_lBufferSize);
	string t_oStrContent((char*)t_pArrCharBuffer, t_lBufferSize);
	Json::Value t_oValueSprites;
	Json::Reader t_oReader;

	if(!t_oReader.parse(t_oStrContent, t_oValueSprites)) {
		CCASSERT(false, "Json::Reader Parse error!");
	}
	if (t_pArrCharBuffer) {
		delete [] t_pArrCharBuffer;
		t_pArrCharBuffer = NULL;
	}
	return t_oValueSprites;
}

void ParseUtils::getGameInfo(const Json::Value& obj,int level){
	for(int i=level;i<obj.size();i++){
		const Json::Value tmp_level = obj[i];
		maxscore = tmp_level["MaxScore"].asInt();
		Json::Value tmp_game= tmp_level["GameArray"];
		for(int k=0;k<49;k++){
			game[k]=tmp_game[k]["V"].asInt();
		}
		Json::Value tmp_monster = tmp_level["Monster"];		
		for(int j=0;j<tmp_monster.size();j++){
			int num = tmp_monster[j]["MonsterNum"].asInt();
			int blood = tmp_monster[j]["MonsterBlood"].asInt();
			MonsterSprite* ms = MonsterSprite::createMonsterSprite(num,blood);
			monsters.push_back(ms);
		}
	}
}

void ParseUtils::getGameInfo(const Json::Value& obj)
{
	curlevel = obj["CurLevel"].asInt();
	const Json::Value tmp_obj = obj["Level"];
	getGameInfo(tmp_obj,curlevel);
}

