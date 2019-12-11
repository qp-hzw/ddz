#ifndef SUB_DEFINE
#define SUB_DEFINE


//抢庄模式
#define ROOMRULE_OPTION_ROBTYPE_FREE		0								//自由抢庄
#define ROOMRULE_OPTION_ROBTYPE_JIAOSANFEN	1								//叫三分


//得分模式
#define GAME_SCORE_NUM						2								//得分模式类型数目
#define GAME_SCORE_MODE_CLASSIC				0								//经典模式
#define GAME_SCORE_MODE_CRAZY				1								//疯狂加倍模式

//桌子类型
#define TABLE_MODE_FANG_KA					0								//房卡模式
#define TABLE_MODE_JJC						1								//竞技场模式
#define TABLE_MODE_GOLD						2								//金币模式

//房间底分
#define ROOMRULE_MIN_CELL_SCORE				1								//房间最小底分


//用户数据
#define MAX_CARD_COUNT						42								//最大手牌扑克数目
#define MAX_GAME_SCORE_TYPES				50								//最大游戏手牌倍数种类，不超过50

//无效卡牌
#define INVALID_CARD						0								//无效卡牌

//////////////////////////////////////////////////////////////////////////
//游戏状态（与断线重连相关）
#define GS_WK_FREE				    GAME_STATUS_FREE					//游戏状态-等待开始
#define GS_WK_ROB					GAME_STATUS_PLAY+1					//游戏状态-抢庄
#define GS_WK_ADD_SCORE				GAME_STATUS_PLAY+2					//游戏状态-抢庄
#define GS_WK_ADDSCORE				GAME_STATUS_PLAY+3					//游戏状态-下注
#define GS_WK_JIAO_FEN				GAME_STATUS_PLAY+7					//叫分状态

#define GS_WK_FANZHU				GAME_STATUS_PLAY+3					//游戏状态-反主
#define GS_WK_KOUDI					GAME_STATUS_PLAY+4					//游戏状态-扣底（盖底牌）
#define GS_WK_OUT_CARD				GAME_STATUS_PLAY+5					//游戏状态-出牌
#define GS_WK_XJ_GAMEEND			GAME_STATUS_PLAY+6					//游戏状态-小局结束
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//定时器   --  1.游戏状态 2.消息号 3.定时器  应该在函数中保持统一
#define	IDI_ROB_BANKER					1							//抢庄定时器
#define IDI_ADD_SCORE					2							//下注定时器
#define IDI_MING_PAI					3							//明牌定时器
#define IDI_ADD_SCORE_ROBOT				4							//机器人下注定时器持续时间

#define IDI_KOU_DI						4							//扣底定时器
#define IDI_OUT_CARD					5							//出牌定时器
#define IDI_XJGAME_USER_READY			6							//小局游戏结束用户准备开始定时器
#define IDI_FAN_ZHU_DELAY				7							//反主延迟定时器，客户端处理完发牌动画之后，再给第一个人发送反主提示
#define IDI_ROUND_END_DELAY				8							//一轮结束延迟定时器，客户端处理积分结算动画
#define IDI_ROB_JiaoFen					9							//叫分定时器

//各个定时器持续时间
#define IDI_TIME_ROB_BANKER				10000						//抢庄定时器持续时间
#define IDI_TIME_ADD_SCORE				5000						//下注定时器持续时间
#define IDI_TIME_MING_PAI				3000						//明牌定时器持续时间
#define IDI_TIME_ADD_SCORE_ROBOT		1000						//机器人下注定时器持续时间

#define IDI_TIME_KOU_DI					20000						//扣底定时器持续时间
#define IDI_TIME_OUT_CARD				20000						//出牌定时器持续时间
#define IDI_TIME_XJGAME_USER_READY		8000						//小局结束用户准备定时器持续时间
#define IDI_TIME_FAN_ZHU_DELAY			6000						//反主延迟定时器持续时间
#define IDI_TIME_ROUND_END_DELAY		2000						//一轮结束延迟定时器持续时间
#define IDI_TIME_ROB_JiaoFen			10000						//叫分定时器
///////////////////////////////

///////////////////////////////
//大局游戏结束
#define GAME_CONCLUDE_CONTINUE			0xFF						//大局结束并续费
#define GAME_CONCLUDE_NORMAL			0xFE						//大局直接结束
///////////////////////////////

//////////////////////////////////////////////////////////////////////////
//椅子上的用户状态
#define USER_NULL                0         //用户状态-椅子空--没有人
#define USER_STANDUP             1         //用户状态-站立  --旁观者   --进入房间后默认属性
#define USER_SITDOWN             2         //用户状态-坐下  --游戏玩家 --可以玩游戏的用户
#define USER_ONREADY             3         //用户状态-准备             --都准备后, 房主可以开始游戏  //TODONOW 是否需要增加定时器或者增加房主踢人功能???
#define USER_PLAYING             4         //用户状态-游戏中           --房主点开始后,所有准备过的玩家进入开始状态    
//////////////////////////////////////////////////////////////////////////
//斗地主定义
#define	ROB_TYPE_CALL			1			//叫地主
#define	ROB_TYPE_ROB			2			//抢地主
#define	ROB_STATE_NON_RESPONE	0			//还未开始抢庄
#define	ROB_STATE_PASS			1			//过
#define	ROB_STATE_AGREE			2			//叫/抢

#define JIAOFEN_START			15			//第一个玩家叫分  1111
#define JIAOFEN_3				1<<3		//叫分三分
#define JIAOFEN_2				1<<2		//叫分二分
#define JIAOFEN_1				1<<1		//叫分一分

#define	MING_PAI_TYPE_OUTCARD	1			//出牌明牌
#define	MING_PAI_TYPE_GAMESTART	2			//游戏开始明牌
#define	MING_PAI_TYPE_DEAL		3			//发牌明牌

#define OUT_CARD_FAIL			0									//出牌失败
#define OUT_CARD_SUCCESS		1									//出牌成功

#define OUT_CARD_PASS			1									//出牌过（要不起）
#define OUT_CARD_NORMAL			0									//正常出牌

#define TURN_END				0									//一轮结束
#define TURN_NOT_END			1									//一轮未结束

#define FULL_COUNT				54									//一副全牌数目

#define MAX_WEAVE_NUM			21									//最多组合数（全部为单张）
#define MAX_BOOM_NUM			5									//最多炸弹数（全部为炸弹）

#define HARD_BOOM_TIMES			4									//最多炸弹数（全部为炸弹）
#define RUAN_BOOM_TIMES			2									//最多炸弹数（全部为炸弹）

//排序类型
#define ST_ORDER				0									//大小排序
#define ST_COUNT				1									//数目排序

//升级定义
#define	DEFALUT_UPGRADE_SCORE	80									//默认多少分升级
#define	COLOR_RIGHT				40									//花色权位
#define LEN_SORT_RIGHT			5									//权位数组大小
#define VALUE_ERROR				0x00								//无效卡牌数值定义
#define COLOR_ERROR				0xFF								//无效花色定义
#define BIG_JOKER				0x4F								//大王
#define SMALL_JOKER				0x4E								//小王
#define NORMAL_LEAVE_CARD_NUM	3									//正常扣底卡牌数
#define NORMAL_HAND_CARD_NUM	17									//正常手牌数
#define	MAX_LEAVE_CARD_NUM		3									//最大扣底卡牌数
#define MAX_COLOR_TYPE_NUM		4									//花色种类数，出去大小王
#define MAX_FANZHU_TYPE_NUM		6									//最大的反主种类数（4对级牌 + 大小王）
#define LEN_MAX_INDEX			67									//最大索引（0-12方 13-25梅花 26-38红桃 39-51黑桃 52-53小王大王）
#define SMALL_JOKER_INDEX		65									//小王索引
#define BIG_JOKER_INDEX			66									//大王索引
#define MAX_SCORE_CARD_NUM		24									//最大得分卡牌数（分牌 5 10 K）
#define START_LEVEL_VALUE		2									//初始级牌大小

#define COLOR_MAIN_CARD			0x04								//主牌标志（包括大小王和级牌）
#define COLOR_HEI_TAO			0x03								//黑桃
#define COLOR_HONG_TAO			0x02								//红桃
#define COLOR_MEI_HUA			0x01								//梅花
#define COLOR_FANG_KUAI			0x00								//方块

//斗地主
#define CT_ERROR					0									//错误类型
#define CT_SINGLE					1									//单牌类型
#define CT_DOUBLE					2									//对牌类型
#define CT_THREE					3									//三条类型
#define CT_SINGLE_LINE				4									//单连类型
#define CT_DOUBLE_LINE				5									//对连类型
#define CT_THREE_LINE				6									//三连类型
#define CT_THREE_LINE_TAKE_ONE		7									//三带一单
#define CT_THREE_LINE_TAKE_TWO		8									//三带一对
#define CT_FOUR_LINE_TAKE_ONE		9									//四带两单
#define CT_FOUR_LINE_TAKE_TWO		10									//四带两对
#define CT_RUAN_BOMB				11									//软炸弹
#define CT_BOMB_CARD				12									//炸弹类型
#define CT_LAIZI_BOMB				13									//赖子炸弹
#define CT_MISSILE_CARD				14									//火箭类型

//升级
#define CT_SAME_2				2									//对牌类型
#define CT_TRACKOR_2			4									//2拖拉机型
#define CT_TRACKOR_3			6									//3拖拉机型
#define CT_TRACKOR_4			8									//4拖拉机型
#define CT_TRACKOR_5			10									//5拖拉机型
#define CT_TRACKOR_6			12									//6拖拉机型
#define CT_TRACKOR_7			14									//7拖拉机型
#define CT_TRACKOR_8			16									//8拖拉机型
#define CT_TRACKOR_9			18									//9拖拉机型
#define CT_TRACKOR_10			20									//10拖拉机型
#define CT_TRACKOR_11			22									//11拖拉机型
#define CT_TRACKOR_12			24									//12拖拉机型
#define CT_TRACKOR_13			26									//13拖拉机型


#define CT_THROW_CARD			15									//甩牌类型

#define MAX_PACK				4									//最大副数		TODO 不知道具体干啥了？？？
#define MAX_TRACKOR				18									//最大拖拉机数
#define INDEX_OUT				0									//出牌索引
#define INDEX_HAND				1									//手牌索引
#define INDEX_FIRST				2									//首出索引

//分数定义
#define	SCORE_DA_GUANG			0									//大光分数
#define	SCORE_XIAO_GUANG		40									//小光分数
#define	SCORE_CHANGE_BANKER		80									//换庄分数
#define	SCORE_UPGRADE			120									//升级分数
#define	SCORE_UPGRADE_ONE		40									//升一级需要得分数

//升级定义
#define	LEVEL_DA_GUANG			3									//大光分数
#define	LEVEL_XIAO_GUANG		2									//小光分数
#define	LEVEL_CHANGE_BANKER		1									//换庄分数
#define	LEVEL_UPGRADE_ONE		40									//升一级需要得分数

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//连牌信息
struct tagSameDataInfo
{
	BYTE							cbCardCount;						//扑克数目
	BYTE							cbBlockCount;						//牌段数目
	BYTE							cbSameData[MAX_CARD_COUNT];			//拖拉机牌
};

//拖拉机信息
struct tagTractorDataInfo
{
	BYTE							cbCardCount;						//扑克数目
	BYTE							cbTractorCount;						//拖牌个数
	BYTE							cbTractorMaxIndex;					//索引位置
	BYTE							cbTractorMaxLength;					//最大长度
	BYTE							cbTractorData[MAX_CARD_COUNT*3/2];	//拖拉机牌
};

//炸弹信息
struct tagBoomInfo
{
	BYTE		cbBoomType[MAX_BOOM_NUM];	//炸弹类型
	BYTE		cbBoomNum;					//炸弹个数
};

//玩家可以出的卡牌信息
struct tagOutCardNotify
{
	BYTE		cbOutCardNum;						//玩家必须出牌的数目
	BYTE		cbMustOutCard[MAX_CARD_COUNT];		//玩家必须出的卡牌
	BYTE		cbOptionalCard[MAX_CARD_COUNT];		//玩家可选出的卡牌
};

//分析结构
struct tagAnalyseResult
{
	BYTE 							cbFourCount;						//四张数目
	BYTE 							cbThreeCount;						//三张数目
	BYTE 							cbDoubleCount;						//两张数目
	BYTE							cbSignedCount;						//单张数目
	BYTE							cbFourCardData[60];					//四张扑克
	BYTE							cbThreeCardData[200];				//三张扑克
	BYTE							cbDoubleCardData[200];				//两张扑克
	BYTE							cbSignedCardData[200];				//单张扑克
	bool							cbLaiZi;							//赖子类型
};

//记牌器结构
struct tagCardRecorder
{
	BYTE							TwoCount;					//牌2的张数
	BYTE							ThreeCount;					//牌3的张数
	BYTE							FourCount;					//牌4的张数
	BYTE							FiveCount;					//牌5的张数
	BYTE							SixCount;					//牌6的张数
	BYTE							SevenCount;					//牌7的张数
	BYTE							EightCount;					//牌8的张数
	BYTE							NineCount;					//牌9的张数
	BYTE							TenCount;					//牌10的张数
	BYTE							JCount;						//牌J的张数
	BYTE							QCount;						//牌Q的张数
	BYTE							KCount;						//牌K的张数
	BYTE							ACount;						//牌A的张数
	BYTE							SmallJokerCount;			//牌小王的张数
	BYTE							BigJokerCount;				//牌大王的张数
};

//房间规则
struct tagTableCfg
{
	//tagCommonRoomRule com_rule; //通用房间规则
	//MSG_SUB_ROOM_RULE sub_rule; //子游戏特有房间规则
};



//==============================机器人相关==================================================

//手牌组合枚举
enum CardGroupType
{
	cgERROR = -1,						            //错误类型
	cgZERO = 0,						                //不出类型
	cgSINGLE = 1,									//单牌类型
	cgDOUBLE = 2,									//对牌类型
	cgTHREE = 3,									//三条类型
	cgSINGLE_LINE = 4,								//单连类型
	cgDOUBLE_LINE = 5,								//对连类型
	cgTHREE_LINE = 6,								//三连类型
	cgTHREE_TAKE_ONE = 7,							//三带一单
	cgTHREE_TAKE_TWO = 8,							//三带一对
	cgTHREE_TAKE_ONE_LINE = 9,						//三带一单连
	cgTHREE_TAKE_TWO_LINE = 10,						//三带一对连
	cgFOUR_TAKE_ONE = 11,							//四带两单
	cgFOUR_TAKE_TWO = 12,							//四带两对
	cgBOMB_CARD = 13,							    //炸弹类型
	cgKING_CARD = 14								//王炸类型
};

//最多手牌
#define HandCardMaxLen 20
//价值最小值
#define MinCardsValue -25
//价值最大值
#define MaxCardsValue 106

#endif