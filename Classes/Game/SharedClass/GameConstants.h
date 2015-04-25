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


#ifndef GameConstants_h
#define GameConstants_h
#include "cocos2d.h"
#include "GameState.h"
using namespace cocos2d;




#define DESIGN_RESOLUTION_480X320    0
#define DESIGN_RESOLUTION_960X640   1
#define DESIGN_RESOLUTION_2048X1536  2


typedef struct tagResource
{
    cocos2d::CCSize size;
    char directory[100];
}Resource;

static void test()
{return;}
static Resource resource1  =  { cocos2d::CCSizeMake(320, 480),   "sd" };
static Resource resource2  =  { cocos2d::CCSizeMake(640,960), "hd1" };
static Resource resource3  =  { cocos2d::CCSizeMake(1280,1920), "hd2" };
static cocos2d::CCSize TARGET_DESIGN_RESOLUTION_SIZE = cocos2d::CCSizeMake(640,960);

#define GAME_SHARED_HANDLER  GameState::sharedGameState()

#define kLightningPowerUpTag 9877
#define kShakeActionTag 9879
#define kTag_TimesUp   701
#define kTag_coins_Board 702
#define kGemSize  75.5  //89.5
#define kBoardWidth  7
#define kBoardHeight  7 // 8
#define kNumTotalGems  kBoardWidth * kBoardHeight
#define kTimeBetweenGemAdds  5 //16
#define kTotalGameTime  120.0f
#define kIntroTime  1.8f
#define kNumRemovalFrames  9
#define kDelayBeforeHint  7.0f
#define kMaxTimeBetweenConsecutiveMoves  1.0f
#define kGameOverGemSpeed  0.1f
#define kGameOverGemAcceleration  0.005
#define kGameUniqueCandies 7
#define KTAG_GamePauseButtonMenu 189
enum kBoardType
{
    kBoardTypeGem0 = 0,
    kBoardTypeGem1 = 1,
    kBoardTypeGem2 = 2 ,
    kBoardTypeGem3 = 3,
    kBoardTypeGem4 = 4,
    kBoardTypeGem5 = 5,
    kBoardTypePup0 = 9,
};

#define NET_WORK_ERROR "Unable to connect"
#define GAME_LABEL_FONT_NATHAN "fonts/Nathan Expandet"
#define GAME_LABEL_FONT_SNIGLET "fonts/Sniglet"
#define GAME_LABEL_FONT_ARIAL "fonts/Arial-bold"


// level meta data plist file

#define LEVEL_PROGRESS "LevelProgress"
#define LEVEL_INFO_PLIST "game.json"

#define Total_Life_Refresh_Time_In_sec 1200 //one hour
#define MAX_LIVES_COUNT 5 //one hour

  //Play menu tags and constants below
#define KTAG_LAYERING_PARAM  20

#define KTAG_PLAYMENU_BLACKbg   117

#define EMOJI_STATE_UNLOCKED 50

#define KTAG_PLYMENU_COINSLBL 60
#define KTAG_Life_Count 61
#define COINS_COUNT_TOTAL "TotalCoinsCount"

  //SocialPuzzle popup layer tags and constants

#define KTAG_EMOJIPOPUP_BackSpriteTag    302

#define KTAG_SOCIALPOPUP_LAYER   352
   //SocialPuzzle manager tags and constants


// Shared Class Constanst
#define totalScore_Label            "Total Score"
#define totalCoins_Label            "Total Coins"
#define longestStreak_Label         "Longest Streak"
#define highestScore_Label          "Highest Score"

// Level selection Tag

#define LEVEL_ONE   1
#define LEVEL_TWO   2
#define LEVEL_THREE 3
#define LEVEL_FOUR  4
#define LEVEL_FIVE  5
#define LEVEL_SIX   6
#define LEVEL_SEVEN 7
#define LEVEL_EIGHT 8
#define LEVEL_NINE  9
#define LEVEL_TEN   10

//MarkSprite Tag VAlue

#define MARK_SPRITE_ONE     11
#define MARK_SPRITE_TWO     12
#define MARK_SPRITE_THREE   13
#define MARK_SPRITE_FOUR    14
#define MARK_SPRITE_FIVE    15
#define MARK_SPRITE_SIX     16
#define MARK_SPRITE_SEVEN   17
#define MARK_SPRITE_EIGHT   18
#define MARK_SPRITE_NINE    19
#define MARK_SPRITE_TEN     20

//Facebook Constants

enum eGameState
{
    kGAMESTATE_FRONTSCREEN_LOGGEDOUT,
    kGAMESTATE_FRONTSCREEN_LOGGEDIN_PREPARING,
    kGAMESTATE_FRONTSCREEN_LOGGEDIN_READY,
    kGAMESTATE_FRONTSCREEN_LOGGEDIN_LOADING,
    kGAMESTATE_FRONTSCREEN_NOSOCIAL_LOADING,
    kGAMESTATE_FRONTSCREEN_NOSOCIAL_READY,
    kGAMESTATE_FRONTSCREEN_SCORES,
    kGAMESTATE_PLAYING,
    kGAMESTATE_PLAYING_NOSOCIAL,
    kGAMESTATE_PLAYING_GAMEOVER,
    kGAMESTATE_PLAYING_GAMEOVER_NOSOCIAL,
};

enum eGameAchievements
{
    kACHIEVEMENT_SCORE50 = 0,
    kACHIEVEMENT_SCORE100,
    kACHIEVEMENT_SCORE150,
    kACHIEVEMENT_SCORE200,
    kACHIEVEMENT_SCOREx3,
    kACHIEVEMENT_MAX
};

enum eFBRequestType
{
    kReqTypeAskLife = 1,
    kReqTypeGiveLife = 2
};



#define APP_HANDLED_URL "APP_HANDLED_URL"
#define APP_HANDLED_URL_IOS @"APP_HANDLED_URL"
//#define kuFBAppID  189433917853064

#define N_USER_LOG_STATUS_CHANGED "N_USER_LOG_STATUS_CHANGED"

#define N_SCORE_DATA_UPDATED "N_SCORE_DATA_UPDATED"
#define N_REQUESTS_DATA_UPDATED "N_REQUESTS_DATA_UPDATED"
#define N_REQUESTS_DATA_UPDATING "N_REQUESTS_DATA_UPDATING" //sending call for update

#define N_FB_CUSTIMIZED "N_FB_CUSTIMIZED" //sending call for update

#define REQUEST_UPDATE_DELAY 10

//Game shared images
#define BUTTON_START_GAME "SocialUI/btn_resume.png"
#define BUTTON_START_GAME_PRESSED "SocialUI/btn_resume_pressed.png"
#define BUTTON_RESTART_GAME "SocialUI/btn_restart.png"
#define BUTTON_RESTART_GAME_PRESSED "SocialUI/btn_restart_pressed.png"
#define BUTTON_QUIT_GAME "SocialUI/btn_quit.png"
#define BUTTON_QUIT_GAME_PRESSED "SocialUI/btn_quit_pressed.png"
#define BUTTON_PAUSE "SocialUI/btn_p.png"
#define BUTTON_PAUSE_PRESSED "SocialUI/btn_p_pressed.png"
#define BUTTON_CLOSE_NORMAL "SocialUI/buttonClose.png"
#define BUTTON_CLOSE_PRESSED "SocialUI/buttonClosePressed.png"
//aduio

#define BUTTON_CLICKED "Sounds/button_clicked.mp3"

//fonts

#define AIRBOLD  "ArialBold"

//Timer Constant

#define NEXT_LIFE_IN "Next Life In "
#define INIT_TIME "initTime"


#define LEVEL_KEY_PREFIX "level"
#define TOTAL_LEVELS 10

#define N_LIFE_COUNT_UPDATED "N_LIFE_COUNT_UPDATED"


#endif
