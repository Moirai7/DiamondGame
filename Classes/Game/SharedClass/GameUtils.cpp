
#include "GameUtils.h"
#include "GameConstants.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
using namespace std;

Size GameUtils::getDesignResolution() {

  return TARGET_DESIGN_RESOLUTION_SIZE;
}

int GameUtils::getIntFromCCString(CCString *str)
{
    
    return atoi(str->getCString());
    
}

CCString* GameUtils::getCCStringFromInt(int num)
{
    char key[100];
    sprintf(key,"%d",num);
    CCString *str = new CCString(key);
    str->autorelease();
    return str;
}

CCString * GameUtils::getCCString(const char* arr)
{
	CCString *str = new CCString(arr);
	str->autorelease();
	return str;
}

bool GameUtils::isStringEqual(const char *pStr1, const char *pStr2)
{
    return strcmp(pStr1, pStr2) == 0;
}

bool GameUtils::isValid(const char* value)
{
	if(!value || isStringEqual(value, ""))
	{
		return false;
	}
	return true;
}
std::string GameUtils::getStdStringFromInt(int value)
{
    char key[100];
    sprintf(key,"%d",value);
    return std::string(key);
}

std::string GameUtils::getStdStringFromLongInt(int64_t value)
{
	 //CCLog("shauket:: vaulue %d",value);
    char key[100];
    sprintf(key,"%I64u",value);
    return std::string(key);
}

CCString* GameUtils::getCCStringFromDouble(double value)
{

	char key[100];
	sprintf(key, "%f", value);
	CCString *str = new CCString(key);
	str->autorelease();
	return str;
}

