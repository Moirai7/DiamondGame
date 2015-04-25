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

#ifndef match3_PlayMenu_h
#define match3_PlayMenu_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace std;

class PlayMenu: public CCLayer
{
private:
    CCLabelTTF*timerForLife;
    int playMenuStatus;
    ScrollView *scView1,*scView2;
    bool iskeybackclicked;
    bool quitPopUpEnable;
    bool randomForEmojiPlacement;
    int arraytobeRandomlocked[100];
    int arraytobeRandom[100];
    
    CCMenu *pendingRequestMenu_;
    CCSize winSize;

//    CCMenuItemImage * facebookMenuItem;
public:
    void timerForLifeCheck(float dt);
    //void keyBackClicked();

    virtual bool init();
  
    void setupGUI();
    void play(Ref* r);
    void updateLifeCountLabel();
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
  
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
  
    virtual void onEnter();
    virtual void onExit();

    // implement the "static node()" method manually
    CREATE_FUNC(PlayMenu);
   // void keyBackClicked(void);
};



#endif
