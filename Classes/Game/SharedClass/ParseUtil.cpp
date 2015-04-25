/****************************************************************************

Match 3 Source Code

Copyright (c) 2014 GameAcademy.com

www.GameAcademy.com

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

****************************************************************************/

#include "ParseUtil.h"
#include "rapidjson/document.h"
#include "cocos2d.h"
#include "GameUtils.h"
using namespace rapidjson;
using namespace cocos2d;

CCObject* ParseUtils::parse(const Value& obj)
{
	if(obj.IsString())
	{
		return GameUtils::getCCString(obj.GetString());
	}
	else if(obj.IsArray())
	{
		CCArray *array = new CCArray();
		for (rapidjson::SizeType i=0; i<obj.Size(); i++)
		{
			const Value& dic1 = obj[i];
			CCObject *convertedObject = ParseUtils::parse(dic1);
			array->addObject(convertedObject);
		}
		array->autorelease();
		return array;
	}
	else if(obj.IsObject())
	{
		CCDictionary * dictionary = new CCDictionary();
		CCArray *allKeys = obj.allKeys();
		for(int i=0 ; i<allKeys->count() ; i++)
		{
			const Value& value = obj[((CCString*)allKeys->objectAtIndex(i))->getCString()];
			CCObject *convertedValue = ParseUtils::parse(value);
			CCString *convertedKey = GameUtils::getCCString(((CCString*)allKeys->objectAtIndex(i))->getCString());
			dictionary->setObject(convertedValue, convertedKey->getCString());
		}
		dictionary->autorelease();
		return dictionary;
	}
	else
	{
		return GameUtils::getCCString(obj.GetString());
	}
}
