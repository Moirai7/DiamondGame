

#ifndef GameUtils_h
#define GameUtils_h

#include "cocos2d.h"
#include "Game/Models/MonsterSprite.h"

#include "Utils/json.h"
using namespace Json;
using namespace cocos2d;

class GameUtils : public CCObject
{
public:
	
	static Size getDesignResolution();
	static int getIntFromCCString(CCString *str);
	static CCString *getCCStringFromInt(int num);
	static CCString *getCCString(const char* arr);
	static bool isValid(const char* value);
	static bool isStringEqual(const char *pStr1, const char *pStr2);
	static std::string getStdStringFromInt(int value);
	static std::string getStdStringFromLongInt(int64_t value);
	static CCString* getCCStringFromDouble(double value);

};


#endif
