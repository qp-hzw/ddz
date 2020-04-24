#ifndef GAME_CONFIG_L
#define GAME_CONFIG_L

#include <windows.h>
#include "GamePlayerConfig.h"
#include "GameCardConfig.h"
#include "GameInterface.h"
#include "GameData.h"
#include "SubGameRule.h"
#include "cfg.h"

//从配置文件读取的玩法规则
typedef struct _tagCommonProfilePara
{
	BYTE	game_normal_times[MAX_GAME_SCORE_TYPES];	// 经典得分模式，从配置文件中获取得分的倍数，目前27种
	BYTE	game_crazy_times[MAX_GAME_SCORE_TYPES];		// 疯狂得分模式，从配置文件中获取得分的倍数，目前27种

}COM_PROFILE_RULE;

//通用玩家配置，从配置文件中读取
typedef struct _tagCommonPlayerPara
{
	PLAYER_TYPE			type;					// 玩家TYPE，数组，每个数代表某个用户类型玩家数目、手牌数目			
}COM_PLAYER_CONFIG;

//通用卡牌配置，从配置文件中读取
typedef struct _tagCommonCardPara
{
	CARD_GOURP group[MAX_CARD_GROUP_NUM];		//卡牌的牌组（桃杏梅方）
	DWORD groupNum;								//牌组的个数
	CARD_DESC game_cards[MAX_TOTAL_CARD_NUM];	//游戏卡牌
	DWORD game_cards_num;						//实际卡牌总数
}COM_CARD_CONFIG;


//在dll加载的时候 读取配置文件
class CGameConfig
{
public:
	static std::string      cfg_file;              // 配置文件

	static COM_PROFILE_RULE	   gComGamePara;	  	// 游戏配置
	static COM_CARD_CONFIG		gComCardPara;		// 卡牌配置
	static COM_PLAYER_CONFIG	gComPlayerPara;		// 玩家配置

public:
	CGameConfig(void);
	~CGameConfig(void);

private:
	//获取游戏卡牌配置
	static int CreateGameCards();

	//获取游戏玩家配置
	static int CreatePlayers();

public:
	//从配置文件中获取通用的游戏配置，在游戏开始前赋给全局变量
	static int LoadGameCommonConfig();
	//获取配置文件名称
	static DWORD BindCfgFile();
};
#endif