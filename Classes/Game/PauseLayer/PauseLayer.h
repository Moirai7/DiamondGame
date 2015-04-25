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

#ifndef __match3__PauseLayer__
#define __match3__PauseLayer__


#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

class PauseLayer: public CCLayer
{
private:
    CCSprite* background;
    void setupGUI();
public:
    
    bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void onTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    CCSize winSize;
    virtual bool init();
    void resumeGame(Ref* sender);
    void quitGame(Ref* sender);
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    virtual void onEnter();
    virtual void onExit();
    
    //function
       
    
    // implement the "static node()" method manually
    CREATE_FUNC(PauseLayer);
};

#endif /* defined(__match3__PauseLayer__) */
