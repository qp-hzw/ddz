#ifndef CMD_CORRESPOND_HEAD_FILE
#define CMD_CORRESPOND_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//ע������

#define MDM_CS_REGISTER				1									//����ע��

//����ע��
#define SUB_CS_C_REGISTER_PLAZA		100									//ע��㳡
#define SUB_CS_C_REGISTER_SERVER	101									//ע�᷿��

#define SUB_CS_S_CHECK_LICENSE		300									//�����Ȩ
#define SUB_CS_S_CHECK_LICENSE_RES	301									//��Ȩ����


//ע����
#define SUB_CS_S_REGISTER_FAILURE	200									//ע��ʧ��

//////////////////////////////////////////////////////////////////////////////////

//ע��㳡
struct CMD_CS_C_RegisterPlaza
{
	TCHAR							szServerAddr[32];					//��½�����ַ		//lee��ֻ��������У��ģ�֤����½���Ĵ���
	TCHAR							szServerName[LEN_SERVER];			//��½��������
};

//ע����Ϸ
struct CMD_CS_C_RegisterServer
{
	WORD							wKindID;							//��������
	WORD							wNodeID;							//�ڵ�����
	WORD							wSortID;							//��������
	DWORD							dwServerID;							//��������
	WORD							wGameServerPort;					//����˿�		//��Ϊ��Ϸ���˿�
	DWORD							dwOnLineCount;						//��������
	DWORD							dwFullCount;						//��Ա����
	TCHAR							szGameServerAddr[32];				//�����ַ		//��Ϊ��Ϸ����ַ
	TCHAR							szServerName[LEN_SERVER];			//��������

	//������Ϣ
	SCORE							lServerScore;						//��Ԫ����
	SCORE							lMinServerScore;					//��Ҫ����
};

//ע��ʧ��
struct CMD_CS_S_RegisterFailure
{
	LONG							lErrorCode;							//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};


//��Ȩ����
struct CMD_CS_S_CheckLicense
{
	TCHAR							szApp[128];							//
	TCHAR							szToken[128];						//
	LONG							time;
};

//��Ȩ���
struct CMD_CS_S_CheckLicense_Res
{
	TCHAR							szToken[128];						//
	WORD							result;
};

//////////////////////////////////////////////////////////////////////////////////
//������Ϣ

#define MDM_CS_SERVICE_INFO			2									//������Ϣ

//������Ϣ
#define SUB_CS_C_SERVER_ONLINE		1									//��������
#define SUB_CS_C_SERVER_MODIFY		2									//�����޸�

//�㳡����
#define SUB_CS_S_PLAZA_INFO			100									//�㳡��Ϣ
#define SUB_CS_S_PLAZA_INSERT		101									//�㳡�б�
#define SUB_CS_S_PLAZA_REMOVE		103									//�㳡ɾ��
#define SUB_CS_S_PLAZA_FINISH		104									//�㳡���

//��������
#define SUB_CS_S_SERVER_INFO		110									//������Ϣ
#define SUB_CS_S_SERVER_ONLINE		111									//��������
#define SUB_CS_S_SERVER_INSERT		112									//�����б�
#define SUB_CS_S_SERVER_MODIFY		113									//�����޸�
#define SUB_CS_S_SERVER_REMOVE		114									//����ɾ��
#define SUB_CS_S_SERVER_FINISH		115									//�������

//////////////////////////////////////////////////////////////////////////////////

//��������
struct CMD_CS_C_ServerModify
{
	WORD							wSortID;							//��������
	WORD							wKindID;							//��������
	WORD							wNodeID;							//�ڵ�����
	WORD							wServerPort;						//����˿�
	DWORD							dwOnLineCount;						//��������
	DWORD							dwFullCount;						//��Ա����
	TCHAR							szServerAddr[32];					//�����ַ
	TCHAR							szServerName[LEN_SERVER];			//��������

	//������Ϣ
	SCORE							lServerScore;						//��Ԫ����
	SCORE							lMinServerScore;					//��Ҫ����
};

//��������
struct CMD_CS_C_ServerOnLine
{
	DWORD							dwOnLineCount;						//��������
};

//////////////////////////////////////////////////////////////////////////////////

//�㳡ɾ��
struct CMD_CS_S_PlazaRemove
{
	DWORD							dwPlazaID;							//�㳡��ʶ
};

//��������
struct CMD_CS_S_ServerOnLine
{
	DWORD							wServerID;							//�����ʶ
	DWORD							dwOnLineCount;						//��������
};

//�����޸�
struct CMD_CS_S_ServerModify
{
	WORD							wSortID;							//��������
	WORD							wKindID;							//��������
	WORD							wNodeID;							//�ڵ�����
	DWORD							wServerID;							//�����ʶ
	WORD							wServerPort;						//����˿�
	DWORD							dwOnLineCount;						//��������
	DWORD							dwFullCount;						//��Ա����
	TCHAR							szServerAddr[32];					//�����ַ
	TCHAR							szServerName[LEN_SERVER];			//��������

	//������Ϣ
	SCORE							lServerScore;						//��Ԫ����
	SCORE							lMinServerScore;					//��Ҫ����
};

//����ɾ��
struct CMD_CS_S_ServerRemove
{
	DWORD							wServerID;							//�����ʶ
};

//////////////////////////////////////////////////////////////////////////////////
//�û�����

#define MDM_CS_USER_COLLECT			3									//�û�����

//�û�״̬
#define SUB_CS_C_USER_ENTER			1									//�û�����
#define SUB_CS_C_USER_LEAVE			2									//�û��뿪
#define SUB_CS_C_USER_FINISH		3									//�û����
#define SUB_CS_C_USER_OFFLINE		4									//�û�����  game->correspond
// TODONOW ��¼��û�� ����Ϣ��3�Ĵ���, ��������Ϣ�ŵ�����Ϣ��Ϊ2 ֮���޸�
#define SUB_CS_C_USER_OFFLINE_B		5									//�û�����  correspond -> (game && logon)

//�û�״̬
#define SUB_CS_S_COLLECT_REQUEST	100									//��������

//////////////////////////////////////////////////////////////////////////////////

//�û�����
struct CMD_CS_C_UserEnter
{
	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwGameID;							//��Ϸ��ʶ
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�

	//������Ϣ
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbMemberOrder;						//��Ա�ȼ�
	BYTE							cbMasterOrder;						//����ȼ�

	//SCORE							lDiamond;							//��ʯ
	//SCORE							lGold;								//���
};

//�û��뿪
struct CMD_CS_C_UserLeave
{
	DWORD							dwUserID;							//�û���ʶ
};

struct STR_SUB_CS_C_USER_OFFLINE
{
	DWORD dwUserID;
	DWORD dwGameID;
};

//////////////////////////////////////////////////////////////////////////////////
//Զ�̷���
#define MDM_CS_REMOTE_SERVICE		4									//Զ�̷���

//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//�������

#define MDM_CS_MANAGER_SERVICE		5									//�������

#define SUB_CS_C_SYSTEM_MESSAGE 	1									//ϵͳ��Ϣ
#define SUB_CS_C_PROPERTY_TRUMPET 	2									//������Ϣ
#define SUB_CS_C_MARQUEE_MESSAGE	3									//�������Ϣyang
#define SUB_CS_C_PRIZE_POOL_DATA    4	                                //�ʽ�����
#define SUB_CS_C_PRIZE_POOL_REWARD 	5	                                //�ʽ��н�

#define SUB_CS_S_SYSTEM_MESSAGE 	100									//ϵͳ��Ϣ
#define SUB_CS_S_PROPERTY_TRUMPET  	200									//������Ϣ
#define SUB_CS_S_MARQUEE_MESSAGE	300									//�������Ϣyang
#define SUB_CS_S_PRIZE_POOL_DATA    400	                                //�ʽ�����
#define SUB_CS_S_PRIZE_POOL_REWARD  500	                                //�ʽ��н�

//��������
struct CMD_CS_S_SendTrumpet
{
	WORD                           wPropertyIndex;                      //�������� 
	DWORD                          dwSendUserID;                         //�û� I D
	DWORD                          TrumpetColor;                        //������ɫ
	TCHAR                          szSendNickName[32];				    //����ǳ� 
	TCHAR                          szTrumpetContent[TRUMPET_MAX_CHAR];  //��������
};

//�������Ϣyang
struct  CMD_CS_C_SendMarquee
{
	WORD							MsgType;										//��Ϣ����
	WORD							MsgID;											//��ϢID(ѡ��Ԥ��)
	TCHAR							szMessage[LEN_USER_CHAT];						//��Ϣ

};

//��Ϣ����yang
enum MarqueeMessageType
{
	MarType_SYS	=	 1	,	//ϵͳ
	MarType_Game		=	 2	,	//�д�
};

#define  WinScore 2000000  //����Ӯ��
//��ϢID
enum MarqueeMessageID
{
	SystemID = 1,				//ϵͳ
	WinID = 2,					//�д�
	KillID = 3,					//���д���
	PrizePoolID = 4,            //�ʽ������
};
//�������Ϣyang
struct  CMD_CS_MarqueeMessage
{
	LONGLONG						StartTime;							//��ʼ����ʱ��
	INT								MsgNumberID;                        //�������ϢID
	WORD							MsgType;							//��Ϣ����
	WORD							MsgID;								//��ϢID(ѡ��Ԥ��)
	WORD							MsgFlag;							//��ʾλ��
	WORD							m_wMarqueeMsgPlayCount;				//��Ϣ���Ŵ���
	WORD							m_wMarqueeMsgApartTime;				//��Ϣ���ż��ʱ��
	WORD							MsgPlayTime;						//��Ϣ����ʱ��
	TCHAR							szMessage[LEN_USER_CHAT];			//��Ϣ
};

//�ʽ���������
struct CMD_CS_C_PrizePoolData
{
	WORD                            wGameID;                             //��ϷID
	WORD                            wRoomID;                             //����ID
	LONGLONG                        lAllPrizePool;                       //�ܲʽ��
};
//�ʽ��н�
struct CMD_CS_C_PrizePoolReward
{
	WORD                            wGameID;                             //��ϷID
	WORD                            wRoomID;                             //����ID
	WORD                            wTableID;                            //�н��������ID
	WORD                            wChairID;                            //�н����ID
	LONGLONG                        lRewardGold;                         //�н����
};

//����ʽ�
struct CMD_CS_RoomPrizePool
{
	WORD                            wGameID;                             //��ϷID
	WORD                            wRoomID;                             //����ID
	WORD                            wTableID;                            //����ID
	LONGLONG                        lPrizePool;                          //�ʽ��
	BYTE                            bIsGamePrize;                        //�Ƿ��ܲʽ�
	BYTE                            bIsRoomPrize;                        //�Ƿ񿪷���ʽ�
	BYTE                            bIsSendMessage;                      //�Ƿ��������
};
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//��̨����
#define MDM_CS_WEB_SERVICE	300											//�������

#define SUB_CS_WEB_CONTROL_INFO			26								//������Ӯ
#define	SUB_CS_WEB_FROZEN_ACCOUNT		25								//�˺Ŷ���
#define	SUB_CS_WEB_UPORDOWNPOINT		27								//���·�
#define SUB_CS_WEB_GAME_LEVEL_CONTROL	28								//��Ϸ�Ѷȿ���
#define SUB_CS_WEB_CONTROL_ROOM_VALUE	29								//���Ʒ���ֵ
#define SUB_CS_WEB_MARQUEE_MANAGER		30								//ϵͳ�����������Ϣ
#define	SUB_CS_WEB_ROOM_CONF_NOTIFY		31								//����޸�
#define SUB_CS_WEB_CANCLE_MARQUEE		32								//ȡ��ϵͳ���������

//������Ӯyang
struct CMD_CS_WEB_ControlInfo
{
	DWORD						dwUserID;		//�û�ID
	SCORE						dwControlScore;	//���Ʒ���
};
struct CMD_CS_WEB_ControlInfoEx
{
	DWORD						dwUserID;		//�û�ID
	SCORE						dwControlScore;	//���Ʒ���
	TCHAR						Token[32];		//��ȫ��Կ
};

//�˺Ŷ���yang
struct CMD_CS_WEB_FrozenAccount
{
	DWORD						dwUserID;		//�û�ID
};
struct CMD_CS_WEB_FrozenAccountEx
{
	DWORD						dwUserID;		//�û�ID
	TCHAR						Token[32];		//��ȫ��Կ
};

//���·�yang
struct CMD_CS_WEB_UpOrDownScore
{
	DWORD						dwUserID;		//�û�ID
	SCORE						dwChangePoint;	//�ı����
};
struct CMD_CS_WEB_UpOrDownScoreEx
{
	DWORD						dwUserID;		//�û�ID
	SCORE						dwChangePoint;	//�ı����
	TCHAR						Token[32];		//��ȫ��Կ

};

//������Ϸ����yang
struct CMD_CS_WEB_ControlGameLevel
{
	DWORD						dwRoomID;		//�û�ID
	DWORD						dwControlLevel;	//�Ѷ�
};
struct CMD_CS_WEB_ControlGameLevelEx
{
	DWORD						dwRoomID;		//�û�ID
	DWORD						dwControlLevel;	//�Ѷ�
	TCHAR						Token[32];		//��ȫ��Կ
};

//���Ʒ���ֵyang
struct CMD_CS_WEB_ControlRoomValue
{
	DWORD						dwRoomID;		//����ID
	SCORE						dwValue;
};
struct CMD_CS_WEB_ControlRoomValueEx
{
	DWORD						dwRoomID;		//����ID
	SCORE						dwValue;
	TCHAR						Token[32];		//��ȫ��Կ
};

//ϵͳ�����������Ϣ
struct CMD_CS_WEB_MarqueeManagerEx
{
	INT								MsgNumberID;                        //�������ϢID
	WORD							MsgID;								//��Ϣ����
	LONGLONG						StartTime;							//��ʼ����ʱ��
	WORD							m_wMarqueeMsgPlayCount;				//��Ϣ���Ŵ���
	WORD							m_wMarqueeMsgApartTime;				//��Ϣ���ż��ʱ��
	TCHAR							szMessage[LEN_USER_CHAT];			//��Ϣ
	TCHAR							Token[32];							//��ȫ��Կ
};

//����޸�yang
struct  CMD_CS_WEB_RoomConfNotify
{
	DWORD						dwGameID;
	DWORD						dwRoomID;
	DWORD						OperationType;							//��Ϣ����
};
struct  CMD_CS_WEB_RoomConfNotifyEx
{
	DWORD						dwGameID;
	DWORD						dwRoomID;
	DWORD						OperationType;							//��Ϣ����
	TCHAR						Token[32];		//��ȫ��Կ
};

//ȡ��ϵͳ���������
struct CMD_CS_WEB_CancleMarqueeMsg
{
	INT							dwSystemMsgID;						//ÿһ����ϢID
};

struct CMD_CS_WEB_CancleMarqueeMsgEx
{
	INT							dwSystemMsgID;						//ÿһ����ϢID
	TCHAR						Token[32];		//��ȫ��Կ
};

//////////////////////////////////////////////////////////////////////////////////

#pragma pack()


#endif