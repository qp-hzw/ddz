#ifndef DEFINE_HEAD_FILE
#define DEFINE_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//人数定义
#define MAX_CHAIR					5									//最大椅子
#define MAX_ANDROID					512									//最大机器

//参数定义
#define INVALID_CHAIR				0xFF								//无效椅子
#define INVALID_TABLE				0xFFFF								//无效桌子

//////////////////////////////////////////////////////////////////////////////////
//积分类型
#define SCORE						LONGLONG							//积分类型
//系统参数
#define LEN_USER_CHAT				128									//聊天长度

//////////////////////////////////////////////////////////////////////////////////
//财富类型
#define TREASURE_MONEY				0									//人民币
#define TREASURE_FK					1									//房卡
#define TREASURE_GOLD				2									//金币
#define TREASURE_DIAMOND			3									//钻石
#define TREASURE_JF					4									//积分

//桌子模式
#define TABLE_MODE_FK				0									//房卡模式
#define TABLE_MODE_MATCH			1									//比赛模式
#define TABLE_MODE_GOLD				2									//金币模式
#define TABLE_MODE_FK_GOLD			3									//房卡金币模式
#define TABLE_MODE_CLUB				4									//牌友圈模式

//请求错误
#define REQUEST_FAILURE_NORMAL		0									//常规原因
#define REQUEST_FAILURE_PASSWORD	3									//密码错误

#define LEN_PRIVATE_TABLE_RULE		256									//自建房规则数据长度
#define NUM_MAX_GOURP_CREATE_ROOM	10									//牌友圈创建的最大房间数

/* ************************************************************************************
**                               数据长度
** ************************************************************************************/
//玩家信息
#define LEN_MD5						33									//加密密码
#define LEN_USERNOTE				32									//备注长度
#define LEN_ACCOUNTS				32									//帐号长度
#define LEN_NICKNAME				32									//昵称长度
#define LEN_PASSWORD				33									//密码长度
#define LEN_GROUP_NAME				32									//社团名字
#define LEN_MY_SIGNATURE            128                                 //个性签名
#define LEN_HEAD_URL				256									//头像地址长度
#define LEN_USER_NOTE				256									//用户备注
#define LEN_MOBILE_PHONE			12									//移动电话
#define LEN_IDENTITY_NUM			19									//证件号码
#define LEN_IDENTITY_NAME			16									//真实名字
#define LEN_IP_ADDR                 32                                  //IP地址长度

//机器标识
#define LEN_MACHINE_ID				33									//序列长度

//列表数据
#define LEN_KIND					32									//类型长度
#define LEN_SERVER					32									//房间长度

//比赛场
#define MAX_MATCH					10									//最大比赛场数
#define MAX_MATCH_TITLE				128									//比赛场标题最大长度
#define MAX_MATCH_STAGE				5									//比赛最多阶段
#define MAX_REWORD_STAGE			5									//最多奖励类型
#define MATCH_START_TYPE_COUNT		0									//人满即开
#define MATCH_START_TYPE_TIME		1									//定时开赛

//////////////////////////////////////////////////////////////////////////////////
//列表数据

//无效属性
#define UD_NULL						0									//无效子项
#define UD_IMAGE					100									//图形子项
#define UD_CUSTOM					200									//自定子项

//基本属性
#define UD_GAME_ID					1									//游戏标识
#define UD_USER_ID					2									//用户标识
#define	UD_NICKNAME					3									//用户昵称

//扩展属性
#define UD_GENDER					10									//用户性别
#define UD_GROUP_NAME				11									//社团名字
#define UD_UNDER_WRITE				12									//个性签名

//状态信息
#define UD_TABLE					20									//游戏桌号
#define UD_CHAIR					21									//椅子号码

//积分信息
#define UD_SCORE					30									//用户分数
#define UD_GRADE					31									//用户成绩
#define UD_USER_MEDAL				32									//用户经验
#define UD_EXPERIENCE				33									//用户经验
#define UD_LOVELINESS				34									//用户魅力
#define UD_WIN_COUNT				35									//胜局盘数
#define UD_LOST_COUNT				36									//输局盘数
#define UD_DRAW_COUNT				37									//和局盘数
#define UD_FLEE_COUNT				38									//逃局盘数
#define UD_PLAY_COUNT				39									//总局盘数

//积分比率
#define UD_WIN_RATE					40									//用户胜率
#define UD_LOST_RATE				41									//用户输率
#define UD_DRAW_RATE				42									//用户和率
#define UD_FLEE_RATE				43									//用户逃率
#define UD_GAME_LEVEL				44									//游戏等级

//扩展信息
#define UD_NOTE_INFO				50									//用户备注
#define UD_LOOKON_USER				51									//旁观用户

//图像列表
#define UD_IMAGE_FLAG				(UD_IMAGE+1)						//用户标志
#define UD_IMAGE_GENDER				(UD_IMAGE+2)						//用户性别
#define UD_IMAGE_STATUS				(UD_IMAGE+3)						//用户状态

//////////////////////////////////////////////////////////////////////////////////
//数据库定义

#define DB_ERROR 					-1  								//处理失败
#define DB_SUCCESS 					0  									//处理成功
#define DB_NEEDMB 					18 									//处理失败

//////////////////////////////////////////////////////////////////////////////////
//道具标示
#define PT_USE_MARK_DOUBLE_SCORE    0x0001								//双倍积分
#define PT_USE_MARK_FOURE_SCORE     0x0002								//四倍积分
#define PT_USE_MARK_GUARDKICK_CARD  0x0010								//防踢道具
#define PT_USE_MARK_POSSESS         0x0020								//附身道具 

#define MAX_PT_MARK                 4                                   //标识数目

//有效范围
#define VALID_TIME_DOUBLE_SCORE     3600                                //有效时间
#define VALID_TIME_FOUR_SCORE       3600                                //有效时间
#define VALID_TIME_GUARDKICK_CARD   3600                                //防踢时间
#define VALID_TIME_POSSESS          3600                                //附身时间
#define VALID_TIME_KICK_BY_MANAGER  3600                                //游戏时间 

//////////////////////////////////////////////////////////////////////////////////
//设备类型
#define DEVICE_TYPE_PC              0x00                                //PC
#define DEVICE_TYPE_ANDROID         0x10                                //Android
#define DEVICE_TYPE_ITOUCH          0x20                                //iTouch
#define DEVICE_TYPE_IPHONE          0x40                                //iPhone
#define DEVICE_TYPE_IPAD            0x80                                //iPad

/////////////////////////////////////////////////////////////////////////////////
//手机定义

//视图模式
#define	VIEW_MODE_ALL				0x0001								//全部可视
#define	VIEW_MODE_PART				0x0002								//部分可视

//信息模式
#define VIEW_INFO_LEVEL_1			0x0010								//部分信息
#define VIEW_INFO_LEVEL_2			0x0020								//部分信息
#define VIEW_INFO_LEVEL_3			0x0040								//部分信息
#define VIEW_INFO_LEVEL_4			0x0080								//部分信息

//其他配置
#define RECVICE_GAME_CHAT			0x0100								//接收聊天
#define RECVICE_ROOM_CHAT			0x0200								//接收聊天
#define RECVICE_ROOM_WHISPER		0x0400								//接收私聊

//行为标识
#define BEHAVIOR_LOGON_NORMAL       0x0000                              //普通登录
#define BEHAVIOR_LOGON_IMMEDIATELY  0x1000                              //立即登录

/////////////////////////////////////////////////////////////////////////////////
//处理结果
#define RESULT_ERROR 					-1  								//处理错误
#define RESULT_SUCCESS 					0  									//处理成功
#define RESULT_FAIL 					1  									//处理失败

/////////////////////////////////////////////////////////////////////////////////
//变化原因
#define SCORE_REASON_WRITE              0                                   //写分变化
#define SCORE_REASON_INSURE             1                                   //银行变化
#define SCORE_REASON_PROPERTY           2                                   //道具变化

/////////////////////////////////////////////////////////////////////////////////
//登录房间失败原因
#define LOGON_FAIL_SERVER_INVALIDATION  200                                 //房间失效

//反馈信息
#define LEN_FEEDBACK_TITLE					32									//反馈标题
#define LEN_FEEDBACK_CONTENT				256									//反馈内容

//消息描述
#define LEN_MESSAGE_DESCRIBE				128									//消息描述

//牌友圈数据
#define LEN_GROUP_NAME						32									//牌友圈名称
#define LEN_GROUP_NOTICE					256									//牌友圈公告
#define LEN_GROUP_MESSAGE					256									//牌友圈聊天信息长度

#define LEN_MAX_RECORD_SIZE				 2*4096								//录像大小
#define LEN_MAX_SERVICE_MEG_SIZE		 50									//客服消息大小


#define SOCKET_TCP_PACKET			1024*4                              //网络缓冲
#endif