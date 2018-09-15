#ifndef CMD_GAME_SERVER_HEAD_FILE
#define CMD_GAME_SERVER_HEAD_FILE

#pragma pack(1)

#pragma region ������

#define LEN_PRIVATE_TABLE_RULE		256									//�Խ����������ݳ���
#define NUM_MAX_GOURP_CREATE_ROOM	10									//����Ȧ��������󷿼���

//������Ϣ
#define DTP_GR_TABLE_PASSWORD		1									//��������

//�û�����
#define DTP_GR_NICK_NAME			10									//�û��ǳ�
#define DTP_GR_GROUP_NAME			11									//��������
#define DTP_GR_UNDER_WRITE			12									//����ǩ��

//������Ϣ
#define DTP_GR_USER_NOTE			20									//�û���ע
#define DTP_GR_CUSTOM_FACE			21									//�Զ�ͷ��

//////////////////////////////////////////////////////////////////////////////////
//TODO ��ʱ�������������Ե������ļ��洢
//����ģʽ
#define TABLE_MODE_FK				0									//����ģʽ
#define TABLE_MODE_JJC				1									//������ģʽ
#define TABLE_MODE_GOLD				2									//���ģʽ
#define TABLE_MODE_GROUP			3									//����Ȧ����ģʽ
#define TABLE_MODE_DW				4									//����ģʽ

//�������
#define REQUEST_FAILURE_NORMAL		0									//����ԭ��
#define REQUEST_FAILURE_NOGOLD		1									//��Ҳ���
#define REQUEST_FAILURE_NOSCORE		2									//���ֲ���
#define REQUEST_FAILURE_PASSWORD	3									//�������
#define REQUEST_FAILURE_END			4									//��Ϸ����
#define REQUEST_FAILURE_NOROOMCARD	5									//��ʯ����

//////////////////////////////////////////////////////////////////////////////////
#pragma endregion

/*
** ��Ҫ����:  ��Ϸ�� �� �������̵� ��Ϣ
** ��ϸ����:  1. ��Ϣ��(�� + ��)     2. �ṹ��
** ��ע˵��:  ����Ϣ�ŵĺ���: 
**            1. ��һ���ֶ�: ������־,        SUB(���ܵ�����Ϣ)�� CMD(���ͳ�ȥ����Ϣ), DBR(�����ݿ�Request)��DBO(���ݿⷵ�ص���Ϣ)
**            2. �ڶ����ֶ�: ��Ϣ����         L: logon;  G:gameserver;  C:client; CP:correspond;  W:web  S:���������Ϸ R:�ͻ�������Ϸ
**            3. �������ֶ�: ����Ϣ�ŵı�־   ����LOGON��Ϊ��¼ģ�����Ϣ
**            4. ֮����ֶ�: ����Ϣ�ŵĺ���   ����Accounts��ʾ�˺ŵ�¼
**           
**            �����淶
**            1. �ṹ�������:  ����Ϣ�ŵ�ǰ���STR, ����SUB_CG_LOGON_ACCOUNTS ��Ϊ STR_SUB_CG_LOGON_ACCOUNTS
**
**            2. DBO, DBR��Ϣ�ŵ�����Ϊ:  ��SUB�滻ΪDBR��DBO����
**
**            3. �����������淶: 1) ָ���p  2)������־SUB  3)����Ϣ�ű�־  4)����Ϣ�ű�־
**
**            4. ����������:  On_������־_����Ϣ������Ϣ��
**
**
*/

#pragma region ����Ϣ��1
/* *********************************************************************************
**						MAIN:1   MDM_LOGON    ��¼ ģ��
** *********************************************************************************/
#define MDM_GR_LOGON				1									//��¼��Ϣ

//��¼
#define SUB_CG_LOGON_USERID			1									//ID ��¼
#define CMD_GC_LOGON_USERID			101									//ID ��¼����

#define CMD_GC_LOGON_GET_USER_INFO	102									//����û�������Ϣ	USERID��¼��Ϸ���ɹ�֮��ᷢ�͸��ͻ���

//////////////////////////////////////////////////////////////////////////////////

//ID ��¼
struct STR_SUB_CG_LOGON_USERID
{
	DWORD							dwUserID;									//�û� I D
	TCHAR							szPassword[LEN_MD5];						//��¼����
	TCHAR							szMachineID[LEN_MACHINE_ID];				//��������
	double							dLongitude;									//��¼��Ϸ��ʱGPS����
	double							dLatitude;									//��¼��Ϸ��ʱGPSγ��
};
//ID ��¼����
struct STR_CMD_GC_LOGON_USERID
{
	LONG							lResultCode;								//�������
	TCHAR							szDescribeString[LEN_MESSAGE_DESCRIBE];		//��Ϣ����
	DWORD							dwOffLineGameID;							//������������ϷID, ��0��ʾ�Ƕ�������
};
#pragma endregion

#pragma region ����Ϣ��199
/* *********************************************************************************
**						MAIN:199   MDM_CONFIG    ��������
** *********************************************************************************/
#define MDM_CONFIG					199									//������Ϣ TODONOW��ɾ��

#define CMD_GC_CONFIG_ROOM			101									//��������		��¼�ɹ���Ϸ�����ظ��ͻ���
#define CMD_GC_CONFIG_PROPERTY		102									//��������		��¼�ɹ���Ϸ�����ظ��ͻ���

//////////////////////////////////////////////////////////////////////////////////

//��������
struct STR_CMD_GC_CONFIG_ROOM
{
	//��������
	WORD							wTableCount;						//������Ŀ
	WORD							wChairCount;						//������Ŀ

	//��������
	WORD							wServerType;						//��������
	DWORD							dwServerRule;						//�������
};

//��������
struct STR_CMD_GC_CONFIG_PROPERTY
{
	BYTE							cbPropertyCount;					//������Ŀ
	tagPropertyInfo					PropertyInfo[MAX_PROPERTY];			//��������
};
#pragma endregion

#pragma region ����Ϣ��2
/* *********************************************************************************
**							MAIN:2   MDM_USER    �û�����
** *********************************************************************************/
#define MDM_USER						2									//�û���Ϣ

//����Ϣ  *��ʾ�ͻ���δ�õ�		#��������		
#define SUB_CG_USER_RULE				1									//*�û�����		TODO ò���������û�IP��ʤ�ʡ������ʡ����֣�Ӧ����Ҫ����
#define SUB_CG_USER_LOOKON				2									//*�Թ�����		TODO �ɹ��޷��أ�ʧ�ܷ���103����ʧ��
#define SUB_CG_USER_SITDOWN				3									//*���������û�����/��������ɹ��󣬷�����������������£�����Ҫ����
#define SUB_CG_USER_STANDUP				4									//#��������		TODO �Ƿ�����ɢ���䣬���͵��Ǹ���Ϣ������15�Ĵ���������
#define SUB_CG_USER_INVITE_USER			5									//*�����û���������
#define CMD_GC_USER_INVITE_USER			6									//*�����û��������ӷ���
#define SUB_CG_USER_REFUSE_SIT  		7									//*�ܾ��������	TODO ���������������ϵͳ��Ϣ�������û�����
#define SUB_CG_USER_KICK_USER			8                                   //*�߳��û�		TODO ���������������ϵͳ��Ϣ���߳��û�

#define SUB_GR_USER_CHAIR_REQ			10                                  //*�������λ��
#define SUB_GR_USER_WAIT_DISTRIBUTE		12									//�ȴ�����		TODO ���û�׼���󣬷��͵Ŀհ����ͻ�����δ����ˣ�

#define SUB_CG_USER_CREATE_FK_ROOM		13									//#������������
#define CMD_GC_USER_CREATE_ROOM_SUCCESS	121									//#�����ɹ�		TODO �����������������Ȧ����ķ��أ���ʵ��������ĳɹ�����Ӧ�ö��ø���Ϣ

#define SUB_CG_USER_JOIN_FK_ROOM		14									//#���뷿������
#define CMD_GC_USER_JOIN_ROOM_SUCCESS	122									//#����ɹ�		���뷿��/���/����Ȧ����ķ���

#define SUB_CG_USER_JOIN_GROUP_ROOM		18									//#��������Ȧ���� ���ؼ��뷿��ɹ�
#define SUB_CG_USER_JOIN_GOLD_ROOM		19									//#�����ҷ���û�п�λ�ý�ҷ������ȴ����� TODO Ϊ�˴���ͨ���ԣ���������ɿͻ�����Ϸ����
#define SUB_GR_JOIN_DW_ROOM				20									//*������泡��������	

#define SUB_GR_GET_TABLELIST			21									//��ȡ�����б�		TODO ָ���Ǿ��������ѿ������б�
#define SUB_GR_GET_TABLELIST_RESULT		131									//�����б���		

#define SUB_GR_LOTTERY_NOTIFY			22									//�齱֪ͨ CMD TODO ���͵Ŀհ�
#define SUB_GR_QUERY_LOTTERY			23									//����齱 SUB
#define SUB_GR_LOTTERY_RESULT			132									//�齱��� CMD

#define SUB_CG_USER_RECONNECT_ROOM		24									//#��������
#define CMD_GC_USER_RECONNECT_ROOM		134									//#������������

#define SUB_CG_USER_CREATE_GROUP_ROOM	25									//#��������Ȧ���䣨ֻ��Ȧ�����ܴ������䣩	TODO ����Ȧ���ڵ�������Ϣ����
#define CMD_GC_USER_CREATE_GROUP_ROOM	205									//#��������Ȧ���䷵��

#define SUB_GR_USER_SCORE				101									//�û�����		TODO ����Ϸ����д�ֺ������������͸��ͻ���
#define SUB_GR_USER_STATUS				102									//�û�״̬		TODO �����û�״̬ʱ�������OnEventUserItemStatus�����ᷢ�͸���Ϣ���ͻ���
#define SUB_GR_REQUEST_FAILURE			103									//����ʧ��		�󲿷�ʧ����Ϣ�����������Ϣ
#define SUB_GR_NOTIFY_MSG				105									//*֪ͨ��Ϣ
#define CMD_GC_USER_MODIFY_TREASURE		106									//�޸��û��Ƹ���Ϣ����

#define SUB_GR_CREATE_OTHER_SUCCESS		129									//*�����˿����ɹ�		TODO ɾ���������˿����ɹ����ڿ����ɹ��У�����Ҫ��������
#define SUB_GR_JOIN_DWTABLE_SUCCESS		133									//*������淿�ɹ�		�߼�������������뷿�䲻ͬ��ò���Ǽ���ɹ��Ϳ�ʼ��Ϸ��

#define CMD_GR_TABLE_INFO				306									//*���¿�����Ϣ		TODO ��Ҫɾ����������Ϣ��Ϊ4������Ϣ��ͬ��
#define SUB_GR_TABLE_CHAT				308									//*��������			TODO SUB��CMD������Ϣ�Ų�һ������Ҫ�޸�

#define SUB_GF_ONLINE_PLAYERS			313									//*��������

//////////////////////////////////////////////////////////////////////////////////////////////////////////

//������������
struct STR_SUB_CG_USER_CREATE_FK_ROOM
{
	BYTE rule[LEN_PRIVATE_TABLE_RULE];						//�����������
};
//��������ɹ�����
struct STR_CMD_GC_USER_CREATE_ROOM_SUCCESS
{
	DWORD							dwPassword;				//��������
	WORD							wChairID;				//�û����Ӻ�
	BYTE							byAllGameCount;			//�ܾ���
	BYTE							byGameMode;				//��Ϸģʽ 0-���� 1-���ӱ�
	BYTE							byZhuangType;			//��ׯģʽ 0-��ׯ 1-��ׯ 2-�̶�ׯ
};

//���뷿������
struct STR_SUB_CG_USER_JOIN_FK_ROOM
{
	DWORD							dwPassword;		//��������
	double							dLongitude;		//����
	double							dLatitude;		//γ��
};
//���뷿��ɹ�����
struct STR_CMD_GC_USER_JOIN_ROOM_SUCCESS
{
	DWORD							dwRoomID;
	WORD							wChairID;
	WORD							wPlayerCount;
	BYTE							byAllGameCount;			//�ܾ���
	BYTE							byGameMode;				//�齫��0-����ģʽ  1-����ģʽ 2-���ģʽ��  ��Ϸģʽ 0-���� 1-���ӱ�
	BYTE							byZhuangType;			//��ׯģʽ 0-��ׯ 1-��ׯ 2-�̶�ׯ
};

//��������
struct STR_SUB_CG_USER_RECONNECT_ROOM
{
	//BYTE	cbMask;			//0-������ 1-����  
};
//������������
struct STR_CMD_GC_USER_RECONNECT_ROOM
{
	BYTE retCode;						//����ֵ��0�ɹ�  1ʧ��
	DWORD dwPassWord;					//��������
	WORD  wChairID;						//���Ӻ�

	
	BYTE cbGameCount;            //��Ϸ����
    BYTE cbPlayerCount;          //�������
    BYTE cbPayType;              //֧����ʽ
    DWORD lSinglePayCost;         //����֧������
    BYTE gameMod;                //��Ϸģʽ
};

//��������Ȧ����
struct STR_SUB_CG_USER_CREATE_GROUP_ROOM
{
	DWORD		dwUserID;								//���������û�ID
	DWORD		dwGroupID;								//����ȦID
	BYTE		rule[LEN_PRIVATE_TABLE_RULE];			//�����������
};
//��������Ȧ����
struct STR_CMD_GC_USER_CREATE_GROUP_ROOM
{
	DWORD       dwResultCode;							 //�����ʶ
	TCHAR		szDescribeString[LEN_MESSAGE_DESCRIBE];	 //������Ϣ
};

//�û�����
struct STR_SUB_CG_USER_STANDUP
{
	WORD							wTableID;							//����λ��		TODO �ͻ������֪������ID��Ӧ��ֻ֪����������Ŷ�
	WORD							wChairID;							//����λ��
	BYTE							cbForceLeave;						//ǿ���뿪
};

//��������Ȧ����
struct STR_SUB_CG_USER_JOIN_GROUP_ROOM
{
	DWORD		dwGroupID;			//����ȦID
	DWORD		dwUserID;			//�û�ID
};

//�޸��û��Ƹ���Ϣ����
struct STR_CMD_GC_USER_MODIFY_TREASURE
{
	SCORE           lUserDiamond;							 //�û���ʯ
	SCORE			lUserGold;								 //�û����
	SCORE			lOpenRoomCard;							 //�û�����
};

//ϵͳ��Ϣ֪ͨ
struct CMD_GR_NotifyMsg
{
	WORD  wChairID;			 //�¼��û����Ӻ�
	TCHAR szDescribe[128];   //��Ϣ����
};

//������淿��
struct CMD_GR_JoinDwRoom
{
	BYTE byDwID;			//����ID ����ʱֻ������ ֵ=1
	BYTE byMode;			//ģʽ��Ԥ��
	BYTE byRule[64];		//Ԥ��
};

//��Ϸ�� ����Ϣ�� MDM_USER 3������Ϣ��SUB_GR_TABLE_CHAT 308   �շ�һ��
struct CMD_GR_TableChat
{
	DWORD dwUserID;				//�û�ID
	DWORD dwChairID;			//�û����Ӻ�
	DWORD dwTableID;			//�����
	DWORD dwMsgID;				//0-99 �Դ����֣�100-199 �Դ����飬255,�Զ�������
	TCHAR szMsg[256];			//�Զ�����������
};

//������淿�ɹ�
struct CMD_GR_Join_DwTable_Success
{
	BYTE byDwID;			//����ID ����ʱֻ������ ֵ=1
	BYTE byMode;			//ģʽ��Ԥ��
	WORD wChairID;			//���Ӻ�
	BYTE byRule[64];		//Ԥ��
};

//�������
struct tagTableCfg
{
	//ͨ������ 
	BYTE	GameMode;				//��Ϸģʽ *ţţ��0-ţţ����ģʽ 1-ţţ���ӱ�   *�齫��0-����ģʽ  1-����ģʽ 2-���ģʽ 
	BYTE	RobBankType;			//��ׯģʽ 0-��ׯ����ע������ׯ�� 1-��ׯ��������ׯ��2-�̶����̶�������ׯ��  3-Ӯׯ��Ӯ������ׯ�� 4-ҡ���Ӷ�ׯ�����ׯ�ң�
	BYTE	GameCount;				//��Ϸ���� 0-���޾�
	BYTE	PlayerCount;			//������� 0-���������ɿ�
	BYTE	cbPayType;				//֧����ʽ��0����֧����1AA��
	DWORD	lSinglePayCost;			//AAÿ������Ҫ�ķ�������֧����Ҫ���ܷ���
	WORD	CellScore;				//�׷�	  Ĭ��Ϊ1
	DWORD	MaxFan;					//�ⶥ���� 0-���ⶥ 
	DWORD	LeaveScore;				//�볡���� 0-������
	DWORD	JoinScore;				//�볡�趨 0-������  
	BYTE	bRefuseSameIP;			//����ͬIP    0-������ 1-����
	BYTE	bDistanceIn300;			//����300��	  0-������ 1-����
	double	dLongitude;				//����	
	double	dLatitude;				//γ��	
	BYTE	bCreateToOther;			//�Ƿ�Ϊ���˿��� 0-�� 1-��
	DWORD	FangZhu;				//����    �����û���ID
	BYTE	bAllowStranger;			//����İ���˼���

	//����Ϸ���й���
	BYTE	ZhangMao;					//��ë
	BYTE	XuanFengGang;				//�����
	BYTE	QiXiaoDui;					//��С��
	BYTE	QingYiSe;					//��һɫ
	BYTE	KaiHuaGang;					//�ܿ�
	BYTE	Other0;
	BYTE	Other1;
	BYTE	Other2;
	BYTE	Other3;
	BYTE	Other4;
	BYTE	Other5;
	BYTE	Other6;
	BYTE	Other7;
};

//������ҷ���
struct CMD_GR_Create_Gold_Room
{
	int dwCellScore;		//��ҳ��׷�
};

//����ɹ�
struct CMD_GR_Join_GoldTable_Success
{
	WORD							wChairID;
};

//���󷿼��б�
struct CMD_GR_GetTableList
{
	BYTE							GameMode;	//��Ϸģʽ
	BYTE							GoldRank;	//��ҵȼ�
	DWORD							dwUserID;	//�û�ID
};

//������Ϣ TOODNOW ��Щ�����϶���Ҫ��д
struct TableInfo
{
	//�������
	DWORD	dwTablePassword;
	DWORD	wCellScore;
	BYTE	byMaxPlayerCount;
	BYTE	byCurrPlayerCount;
	DWORD	dwJoinScore;
	DWORD	dwLeaveScore;
	//��Ϸ����
	BYTE	ZhangMao;					//��ë
	BYTE	XuanFengGang;				//�����
	BYTE	QiXiaoDui;					//��С��
	BYTE	QingYiSe;					//��һɫ
	BYTE	KaiHuaGang;					//�ܿ�
};

//�����б�
struct CMD_GR_TableListResult
{
	BYTE		byCount;				//��������
	BYTE		GoldRank;				//0������ 50-100 1�߼��� 100-500 2������ 500-5000
	TableInfo	tableInfoList[20];
};

//�齱֪ͨ
struct CMD_GR_LotteryNotify
{
	BYTE	byType;					//����
	DWORD	dwTimeLeft;				//��ʱ���룩
};

//����ҡ��
struct CMD_GR_QueryLottery
{
	DWORD	dwUserID;				//�û�ID
	BYTE	byType;					//�齱��ʽ 0-��� 1-ҡ��
};

//�齱���
struct CMD_GR_LotteryResult
{
	BYTE	byType;					//�������  0-û�г鵽 1-�һ��� 2-���  3-���� 4-��ʯ 5-���� ����-����Ԥ��
	BYTE    byIndex;				//0-11
	DWORD	dwRewardCount;			//������������
	TCHAR	szPacketID[128];				//�һ���
	TCHAR	szDescribe[128];		//����
};

//����
struct CMD_GF_XUFEI
{
	WORD							wChairID;
	BYTE							bFangzhu;
};

//�Թ�����
struct CMD_GR_UserLookon
{
	WORD							wTableID;							//����λ��
	WORD							wChairID;							//����λ��
};

//��������
struct CMD_GR_UserSitDown
{
	WORD							wTableID;							//����λ��
	WORD							wChairID;							//����λ��
	TCHAR							szPassword[LEN_PASSWORD];			//��������
};

//�����û� 
struct CMD_GR_UserInvite
{
	WORD							wTableID;							//���Ӻ���
	DWORD							dwUserID;							//�û� I D
};

//�����û����� 
struct CMD_GR_UserInviteReq
{
	WORD							wTableID;							//���Ӻ���
	DWORD							dwUserID;							//�û� I D
};

//�û�����
struct CMD_GR_UserScore
{
	DWORD							dwUserID;							//�û���ʶ
	tagUserScore					UserScore;							//������Ϣ
};

//�û�����
struct CMD_GR_MobileUserScore
{
	DWORD							dwUserID;							//�û���ʶ
	tagMobileUserScore				UserScore;							//������Ϣ
};

//�û�״̬
struct CMD_GR_UserStatus
{
	DWORD							dwUserID;							//�û���ʶ
	tagUserStatus					UserStatus;							//�û�״̬
};

//����ʧ��
struct CMD_GR_RequestFailure
{
	LONG							lErrorCode;							//�������
	TCHAR							szDescribeString[256];				//������Ϣ
};

//�����û�
struct CMD_GR_C_InviteUser
{
	WORD							wTableID;							//���Ӻ���
	DWORD							dwSendUserID;						//�����û�
};

//�����û�
struct CMD_GR_S_InviteUser
{
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//�û��ܾ�����������
struct CMD_GR_UserRepulseSit
{
	WORD							wTableID;							//���Ӻ���
	WORD							wChairID;							//����λ��
	DWORD							dwUserID;							//�û� I D
	DWORD							dwRepulseUserID;					//�û� I D
};

//֪ͨ�û�
struct CMD_GF_OnlinePlayers
{
	WORD wOnlinePlayers;			//��������
};

//////////////////////////////////////////////////////////////////////////////////

//�����־
#define UR_LIMIT_SAME_IP			0x01								//���Ƶ�ַ
#define UR_LIMIT_WIN_RATE			0x02								//����ʤ��
#define UR_LIMIT_FLEE_RATE			0x04								//��������
#define UR_LIMIT_GAME_SCORE			0x08								//���ƻ���

//�û�����
struct CMD_GR_UserRule
{
	BYTE							cbRuleMask;							//��������
	WORD							wMinWinRate;						//���ʤ��
	WORD							wMaxFleeRate;						//�������
	LONG							lMaxGameScore;						//��߷��� 
	LONG							lMinGameScore;						//��ͷ���
};

//�����û���Ϣ
struct CMD_GR_UserInfoReq
{
	DWORD                           dwUserIDReq;                        //�����û�
	WORD							wTablePos;							//����λ��
};

//�����û���Ϣ
struct CMD_GR_ChairUserInfoReq
{
	WORD							wTableID;							//���Ӻ���
	WORD							wChairID;							//����λ��
};
#pragma endregion

#pragma region ����Ϣ��4
/* *********************************************************************************
**						 MAIN:3   MDM_GR_STATUS    ״̬����
** *********************************************************************************/
#define MDM_GR_STATUS				3									//״̬��Ϣ

#define CMD_GR_TABLE_INFO			100									//������Ϣ
#define CMD_GR_TABLE_STATUS			101									//����״̬

//////////////////////////////////////////////////////////////////////////////////

//������Ϣ
struct CMD_GR_TableInfo
{
	WORD							wTableCount;						//������Ŀ
	tagTableStatus					TableStatusArray[512];				//����״̬
};

//����״̬
struct CMD_GR_TableStatus
{
	WORD							wTableID;							//���Ӻ���
	tagTableStatus					TableStatus;						//����״̬
};
#pragma endregion

#pragma region ����Ϣ��6
/* *********************************************************************************
**					 MAIN:6   MDM_GR_MANAGE    ��������
** *********************************************************************************/
#define MDM_GR_MANAGE				6									//��������

#define SUB_GR_SEND_WARNING			1									//���;���
#define SUB_GR_SEND_MESSAGE			2									//������Ϣ
#define SUB_GR_LOOK_USER_IP			3									//�鿴��ַ
#define SUB_GR_KILL_USER			4									//�߳��û�
#define SUB_GR_LIMIT_ACCOUNS		5									//�����ʻ�
#define SUB_GR_SET_USER_RIGHT		6									//Ȩ������

//��������
#define SUB_GR_QUERY_OPTION			7									//��ѯ����
#define SUB_GR_OPTION_SERVER		8									//��������
#define SUB_GR_OPTION_CURRENT		9									//��ǰ����

#define SUB_GR_LIMIT_USER_CHAT		10									//��������

#define SUB_GR_KICK_ALL_USER		11									//�߳��û�
#define SUB_GR_DISMISSGAME		    12									//��ɢ��Ϸ

//////////////////////////////////////////////////////////////////////////////////

//���;���
struct CMD_GR_SendWarning
{
	WORD							wChatLength;						//��Ϣ����
	DWORD							dwTargetUserID;						//Ŀ���û�
	TCHAR							szWarningMessage[LEN_USER_CHAT];	//������Ϣ
};

//ϵͳ��Ϣ
struct CMD_GR_SendMessage
{
	BYTE							cbGame;								//��Ϸ��Ϣ
	BYTE							cbRoom;								//��Ϸ��Ϣ
	BYTE							cbAllRoom;							//��Ϸ��Ϣ
	WORD							wChatLength;						//��Ϣ����
	TCHAR							szSystemMessage[LEN_USER_CHAT];		//ϵͳ��Ϣ
};

//�鿴��ַ
struct CMD_GR_LookUserIP
{
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//�߳��û�
struct CMD_GR_KickUser
{
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//�����ʻ�
struct CMD_GR_LimitAccounts
{
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//Ȩ������
struct CMD_GR_SetUserRight
{
	//Ŀ���û�
	DWORD							dwTargetUserID;						//Ŀ���û�

	//�󶨱���
	BYTE							cbGameRight;						//�ʺ�Ȩ��
	BYTE							cbAccountsRight;					//�ʺ�Ȩ��

	//Ȩ�ޱ仯
	BYTE							cbLimitRoomChat;					//��������
	BYTE							cbLimitGameChat;					//��Ϸ����
	BYTE							cbLimitPlayGame;					//��ϷȨ��
	BYTE							cbLimitSendWisper;					//������Ϣ
	BYTE							cbLimitLookonGame;					//�Թ�Ȩ��
};

//��������
struct CMD_GR_OptionCurrent
{
	DWORD							dwRuleMask;							//��������
	tagServerOptionInfo				ServerOptionInfo;					//��������
};

//��������
struct CMD_GR_ServerOption
{
	tagServerOptionInfo				ServerOptionInfo;					//��������
};

//�߳������û�
struct CMD_GR_KickAllUser
{
	TCHAR							szKickMessage[LEN_USER_CHAT];		//�߳���ʾ
};

//��ɢ��Ϸ
struct CMD_GR_DismissGame
{
	WORD							wDismissTableNum;		            //��ɢ����
};
//////////////////////////////////////////////////////////////////////////////////

//���ñ�־
#define OSF_ROOM_CHAT				1									//��������
#define OSF_GAME_CHAT				2									//��Ϸ����
#define OSF_ROOM_WISPER				3									//����˽��
#define OSF_ENTER_TABLE				4									//������Ϸ
#define OSF_ENTER_SERVER			5									//���뷿��
#define OSF_SEND_BUGLE				12									//��������

//��������
struct CMD_GR_OptionServer
{
	BYTE							cbOptionFlags;						//���ñ�־
	BYTE							cbOptionValue;						//���ñ�־
};

//��������
struct CMD_GR_LimitUserChat
{
	DWORD							dwTargetUserID;						//Ŀ���û�
	BYTE							cbLimitFlags;						//���Ʊ�־
	BYTE							cbLimitValue;						//�������
};
#pragma endregion

#pragma region ����Ϣ��7
/* *********************************************************************************
**						MAIN:7   MDM_GR_MATCH    ��������
** *********************************************************************************/
#define MDM_GR_MATCH				7									//��������

#define SUB_GR_MATCH_FEE			400									//��������
#define SUB_GR_MATCH_NUM			401									//�ȴ�����
#define SUB_GR_LEAVE_MATCH			402									//�˳�����
#define SUB_GR_MATCH_INFO			403									//������Ϣ
#define SUB_GR_MATCH_WAIT_TIP		404									//�ȴ���ʾ
#define SUB_GR_MATCH_RESULT			405									//�������
#define SUB_GR_MATCH_STATUS			406									//����״̬
#define SUB_GR_MATCH_USER_COUNT		407									//��������
#define SUB_GR_MATCH_DESC			408									//��������
////////////////////////////////////////////////////////////////////////////////////
//��������
struct CMD_GR_Match_Num
{
	DWORD							dwWaitting;							//�ȴ�����
	DWORD							dwTotal;							//��������
	DWORD							dwMatchTotal;						//������
};

//������Ϣ
struct CMD_GR_Match_Info
{
	TCHAR							szTitle[4][64];						//��Ϣ����
    WORD							wGameCount;							//��Ϸ����
};

//��ʾ��Ϣ
struct CMD_GR_Match_Wait_Tip
{
	SCORE							lScore;								//��ǰ����
	WORD							wRank;								//��ǰ����
	WORD							wCurTableRank;						//��������
	WORD							wUserCount;							//��ǰ����
	WORD							wPlayingTable;						//��Ϸ����
	TCHAR							szMatchName[LEN_SERVER];			//��������
};

//�������
struct CMD_GR_MatchResult
{
	TCHAR							szDescribe[256];					//�ý�����
	DWORD							dwGold;								//��ҽ���
	DWORD							dwMedal;							//���ƽ���
	DWORD							dwExperience;						//���齱��
};

#define MAX_MATCH_DESC				4									//�������
//��������
struct CMD_GR_MatchDesc
{
	TCHAR							szTitle[MAX_MATCH_DESC][16];		//��Ϣ����
	TCHAR							szDescribe[MAX_MATCH_DESC][64];		//��������
	COLORREF						crTitleColor;						//������ɫ
	COLORREF						crDescribeColor;					//������ɫ
};
#pragma endregion

#pragma region ����Ϣ��8
/* *********************************************************************************
**							MAIN:8   MDM_PROPERTY    ��������
** *********************************************************************************/
#define MDM_PROPERTY					8								

#define SUB_GR_PROPERTY_BUY				300									//*�������
#define SUB_GR_PROPERTY_SUCCESS			301									//*���߳ɹ�
#define SUB_GR_PROPERTY_FAILURE			302									//*����ʧ��
#define SUB_GR_PROPERTY_MESSAGE			303                                 //*������Ϣ
#define SUB_GR_PROPERTY_EFFECT			304                                 //*����ЧӦ
#define SUB_GR_PROPERTY_TRUMPET			305                                 //*������Ϣ
#define SUB_GR_QUERY_USER_PROP			309									//*��ѯ�û�����
#define SUB_GR_USER_PROP_RESULT			310									//*��ѯ�û����߷���
#define SUB_GR_MODIFY_USER_PROP			311									//*����\�����û�����
#define SUB_GR_MODIFY_PROP_RESULT		312									//*�޸ĵ��߽��
///////////////////////////////////////////////////////////////////////////////////////

//�������
struct CMD_GR_C_PropertyBuy
{
	BYTE                            cbRequestArea;						//����Χ
	BYTE							cbConsumeScore;						//��������
	WORD							wItemCount;							//������Ŀ
	WORD							wPropertyIndex;						//��������	
	DWORD							dwTargetUserID;						//ʹ�ö���
};

//���߳ɹ�
struct CMD_GR_S_PropertySuccess
{
	BYTE                            cbRequestArea;						//ʹ�û���
	WORD							wItemCount;							//������Ŀ
	WORD							wPropertyIndex;						//��������
	DWORD							dwSourceUserID;						//Ŀ�����
	DWORD							dwTargetUserID;						//ʹ�ö���
};

//����ʧ��
struct CMD_GR_PropertyFailure
{
	WORD                            wRequestArea;                       //��������
	LONG							lErrorCode;							//�������
	TCHAR							szDescribeString[256];				//������Ϣ
};

//������Ϣ
struct CMD_GR_S_PropertyMessage
{
	//������Ϣ
	WORD                            wPropertyIndex;                     //��������
	WORD                            wPropertyCount;                     //������Ŀ
	DWORD                           dwSourceUserID;                     //Ŀ�����
	DWORD                           dwTargerUserID;                     //ʹ�ö���
};


//����ЧӦ
struct CMD_GR_S_PropertyEffect
{
	DWORD                           wUserID;					        //�� ��I D
	BYTE							cbMemberOrder;						//��Ա�ȼ�
};

//��������
struct CMD_GR_C_SendTrumpet
{
	BYTE                           cbRequestArea;                        //����Χ 
	WORD                           wPropertyIndex;                      //�������� 
	DWORD                          TrumpetColor;                        //������ɫ
	TCHAR                          szTrumpetContent[TRUMPET_MAX_CHAR];  //��������
};

//��������
struct CMD_GR_S_SendTrumpet
{
	WORD                           wPropertyIndex;                      //�������� 
	DWORD                          dwSendUserID;                         //�û� I D
	DWORD                          TrumpetColor;                        //������ɫ
	TCHAR                          szSendNickName[32];				    //����ǳ� 
	TCHAR                          szTrumpetContent[TRUMPET_MAX_CHAR];  //��������
};

//��ѯ�û�����
struct CMD_GR_QueryUserProp
{
	DWORD dwUserID;
};

//��ѯ����
struct CMD_GR_UserPropResult
{
	DWORD dwUserID;			//�û�ID
	WORD  wPropID[5];		//����ID
	LONGLONG lPropCount[5];	//��������
};

//����\�����û�����
struct CMD_GR_ModifyUserProp
{
	BYTE	bAdd;		//0��ʾ���٣�>0��ʾ����
	DWORD	dwUserID;
	WORD	wPropID;
	LONGLONG lPropCount;
};

//�޸ĵ��߽��
struct CMD_GR_ModifyPropResult
{
	WORD	wErrCode;		//������
	DWORD	dwUserID;		//�û�ID
	WORD	wPropID;		//����ID
	LONGLONG lPropCount;	//������ĵ�������
	TCHAR	szDescribe[128]; //����
};
#pragma endregion

#pragma region ����Ϣ��100
/* *********************************************************************************
**							MAIN:100   MDM_G_FRAME    �������
** *********************************************************************************/
#define MDM_G_FRAME					100									//�������

//������Ϣ
#define SUB_RG_FRAME_OPTION					1								//��Ϸ����
#define CMD_GR_FRAME_GAME_OPTION			101								//��Ϸ����
#define CMD_GR_FRAME_GAME_STATUS			102								//��Ϸ״̬

//��ɢ����
#define SUB_RG_FRAME_ASK_DISMISS			2								//�����ɢ����
#define	SUB_RG_FRAME_VOTE_DISMISS			3								//�����ɢ����

#define CMD_GR_FRMAE_VOTE_DISMISS			103								//����Ƿ��ɢ����
#define	CMD_GR_FRAME_DISMISS_RESULT			104								//��ɢ������

//GPS���
#define CMD_GC_COM_CHECK_USER_GPS			105								//У���û�GPSλ����Ϣ����



//���涼�Ǵ����� client��, ��û������
#define SUB_GF_USER_READY				4									//�û�׼��
#define SUB_GF_USER_CHAT				5									//�û�����

#define CMD_GF_SYSTEM_MESSAGE			106									//ϵͳ��Ϣ


//����clientû��
#define SUB_GF_LOOKON_CONFIG			803									//�Թ�����
#define SUB_GF_HEADURL					804									//*ͷ���ַ
#define SUB_GF_XUFEI					805									//*������Ϣ

#define SUB_GF_USER_EXPRESSION			811									//*�û�����
#define SUB_GF_SOUND					812									//*��������
#define SUB_GF_HEAD						813									//*����ͷ��

//��Ϸ��Ϣ
#define CMD_GF_LOOKON_STATUS			803									//*�Թ�״̬

//ϵͳ��Ϣ
#define CMD_GF_SYSTEM_MESSAGE			806									//ϵͳ��Ϣ
#define SUB_GF_ACTION_MESSAGE			901									//������Ϣ
#define SUB_GF_MARQUEE_MESSAGE			902									//�������Ϣ

//////////////////////////////////////////////////////////////////////////////////////

//��Ϸ����
struct CMD_GF_GameOption
{
	BYTE							cbAllowLookon;						//�Թ۱�־
	DWORD							dwFrameVersion;						//��ܰ汾
	DWORD							dwClientVersion;					//��Ϸ�汾
};

//�Թ�����
struct CMD_GF_LookonConfig
{
	DWORD							dwUserID;							//�û���ʶ
	BYTE							cbAllowLookon;						//�����Թ�
};

//�Թ�״̬
struct CMD_GF_LookonStatus
{
	BYTE							cbAllowLookon;						//�����Թ�
};

//��Ϸ����
struct CMD_GF_GameStatus
{
	BYTE							cbGameStatus;						//��Ϸ״̬
	BYTE							cbAllowLookon;						//�Թ۱�־
};

//ͷ���ַ
struct CMD_GF_C_HeadUrl
{
	TCHAR							szHeadUrl[LEN_HEAD_URL];
};

//ͷ���ַ
struct CMD_GF_S_HeadUrl
{
	WORD							wChairID;
	TCHAR							szHeadUrl[LEN_HEAD_URL];
};


//�û�����
struct CMD_GF_C_UserChat
{
	WORD							wChatLength;						//��Ϣ����
	DWORD							dwChatColor;						//��Ϣ��ɫ
	DWORD							dwTargetUserID;						//Ŀ���û�
	TCHAR							szChatString[LEN_USER_CHAT];		//������Ϣ
};

//�û�����
struct CMD_GF_S_UserChat
{
	WORD							wChatLength;						//��Ϣ����
	DWORD							dwChatColor;						//��Ϣ��ɫ
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
	TCHAR							szChatString[LEN_USER_CHAT];		//������Ϣ
};

//�û�����
struct CMD_GF_C_UserExpression
{
	WORD							wItemIndex;							//��������
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//�û�����
struct CMD_GF_S_UserExpression
{
	WORD							wItemIndex;							//��������
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//�����ɢ����
struct STR_SUB_RG_FRAME_ASK_DISMISS
{
	DWORD					dwApplyUserID;					//�����ɢ�û�ID
	BYTE					cbAgree;						//�Ƿ�ͬ���ɢ		0-��ͬ��	1-ͬ��
};
//����Ƿ��ɢ����
struct STR_CMD_GR_FRMAE_VOTE_DISMISS
{
	DWORD					dwVoteUser[5];					//��ҪͶƱ��ɢ�����	//�ͻ���Ŀǰ�����ʾ5����
	TCHAR					szApplyUserNick[LEN_NICKNAME];	//�����ɢ�û��ǳ�
};

//�����ɢ����
struct STR_SUB_RG_FRAME_VOTE_DISMISS
{
	DWORD					dwVoteUserID;					//�����ɢ�����û�ID
	BYTE					cbAgree;						//�Ƿ�ͬ���ɢ		0-��ͬ��	1-ͬ��
};
//��ɢ������
struct STR_CMD_GR_FRAME_DISMISS_RESULT
{
	BYTE					cbDismiss;						//0-δ��ɢ  1-��ɢ�ɹ����û�������	2-��ɢʧ��
	DWORD					dwVoteUserID;					//�����ɢ�����û�ID
	TCHAR					szVoteUserNick[LEN_NICKNAME];	//�����ɢ�û��ǳ�
	BYTE					cbAgree;						//�Ƿ�ͬ���ɢ
};

//У���û�GPSλ����Ϣ����
struct STR_CMD_CG_COM_CHECK_USER_GPS
{
	DWORD					dwFirstUserID;					//��һ�����
	DWORD					dwNextUserID;					//�ڶ������
	double					fDistance;						//������ҵľ���
};
#pragma endregion


#pragma region ����Ϣ��200
/* *********************************************************************************
**							MAIN:200   MDM_GF_GAME    ����Ϸ����
** *********************************************************************************/
#define MDM_GF_GAME					200									//��Ϸ����

#pragma endregion

#pragma pack()

#endif