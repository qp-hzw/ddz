#ifndef CMD_LONGON_HEAD_FILE
#define CMD_LONGON_HEAD_FILE

#pragma pack(1)

/*
** ��Ҫ����:  ��¼�� �� �������̵� ��Ϣ
** ��ϸ����:  1. ��Ϣ��(�� + ��)     2. �ṹ��
** ��ע˵��:  ����Ϣ�ŵĺ���: 
**            1. ��һ���ֶ�: ������־,        SUB(���ܵ�����Ϣ)�� CMD(���ͳ�ȥ����Ϣ), DBR(�����ݿ�Request)��DBO(���ݿⷵ�ص���Ϣ)
**            2. �ڶ����ֶ�: ��Ϣ����         L: logon;  G:gameserver;  C:client; CP:correspond;  W:web
**            3. �������ֶ�: ����Ϣ�ŵı�־   ����LOGON��Ϊ��¼ģ�����Ϣ
**            4. ֮����ֶ�: ����Ϣ�ŵĺ���   ����Accounts��ʾ�˺ŵ�¼
**           
**            �����淶
**            1. �ṹ�������:  ����Ϣ�ŵ�ǰ���STR, ����SUB_CL_LOGON_ACCOUNTS ��Ϊ STR_SUB_CL_LOGON_ACCOUNTS
**
**            2. DBO, DBR��Ϣ�ŵ�����Ϊ:  ��SUB�滻ΪDBR��DBO����
**
**            3. �����������淶: 1) ָ���p  2)������־SUB  3)����Ϣ�ű�־  4)����Ϣ�ű�־
**
**            4. ����������:  On_������־_����Ϣ������Ϣ��
**
**
*/

/* *********************************************************************************
**      MAIN:1   MDM_LOGON    ��¼ ģ��
** *********************************************************************************/
#define MDM_LOGON					1									//����Ϣ

#define SUB_CL_LOGON_ACCOUNTS		1									//�ʺŵ�¼
#define CMD_LC_LOGON_ACCOUNTS		101									//�˺ŵ�¼

#define SUB_CL_LOGON_REGISTER	    2									//�ʺ�ע�ᣨ���ص����˺ŵ�¼��
#define CMD_LC_LOGON_REGISTER		102									//�˺�ע�᷵��

#define SUB_CL_LOGON_PLATFORM		3									//ƽ̨��½����������¼��
#define CMD_LC_LOGON_PLATFORM       103                                 //ƽ̨��½����

#define CMD_LC_LOGON_REPEAT_LOGON	104									//�ظ���¼

#define CMD_LC_LOGON_LOGON_REWARD   105                                 //��¼����������һع� + ���ս�����

#define CMD_LC_LOGON_UPDATE_NOTIFY	106									//������ʾ����¼ + ע�� + ƽ̨��¼�����õ���

//////////////////////////////////////////////////////////////////////////////////

//�ʺŵ�¼  
struct STR_SUB_CL_LOGON_ACCOUNTS
{
	DWORD							dwVersionCheck;						//�汾У�飨32λ = ����汾(8λ) + �ں˰汾(8λ) + ƽ̨�汾(8λ) + ����Ϸ�汾(8λ)��
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};
//�˺ŵ�¼����
struct STR_CMD_LC_LOGON_ACCOUNTS
{
	//�������
	DWORD                           dwResultCode;                       //�����ʶ
	//������Ϣ
	TCHAR							szDescribeString[128];				//������Ϣ

	/* *****************************    �û���Ϣ     ****************************/
	//�û���ʶ
	DWORD							dwUserID;							//�û���ʶ
	//��������
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	BYTE							cbGender;							//�û��Ա�
	WORD							wFaceID;							//ͷ������
	TCHAR							szMySignature[LEN_MY_SIGNATURE];	//����ǩ��
	//��������
	SCORE                           dwUserDiamond;						//�û�Ԫ��
	DWORD							dwUserMedal;						//�û�����
	DWORD							dwExperience;						//������ֵ
	DWORD							dwLoveLiness;						//�û�����
	//�û�Ȩ��
	BYTE                            cbMasterOrder;                      //����Ա�ȼ� 0��ʾ�ǹ���Ա
	BYTE							cbMemberOrder;						//��Ա�ȼ�   0��ʾ�ǹ���Ա
	//SYSTEMTIME						MemberOverDate;						//��Ա����ʱ��
	//��ϵ��ʽ
	TCHAR                           szIdentityName[LEN_IDENTITY_NAME];  //��ʵ����
	TCHAR                           szIdentityNum[LEN_IDENTITY_NUM];    //���֤��
	TCHAR                           szMobilePhone[LEN_MOBILE_PHONE];    //�ֻ�����
	
	/* *****************************    �˺���Ϣ     ****************************/
	TCHAR							szLasLogonIp[LEN_IP_ADDR];			//����¼��ַ
	SYSTEMTIME						LasLogonDate;						//�������ʱ��
	
	/* *****************************    ��������     ****************************/
	//����Ȧ 
	DWORD							dwGroupID;							//���ű�ʶ
	TCHAR							szGroupName[LEN_GROUP_NAME];		//��������

	//�Ƹ���Ϣ
	SCORE							lUserScore;							//�û�����
	SCORE							lUserGold;							//�û����
	SCORE							lUserRoomCard;						//�û�����
};

//ע���ʺ�
struct STR_SUB_CL_LOGON_REGISTER
{
	DWORD							dwVersionCheck;						//�汾У��
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	BYTE							cbGender;							//�û��Ա�
	TCHAR                           strMobilePhone[LEN_MOBILE_PHONE];   //�ֻ����� ����Ϊnull
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};
//�˺�ע�᷵��
struct STR_CMD_LC_LOGON_REGISTER
{
	//�������
	DWORD                           dwResultCode;                       //�����ʶ
	//������Ϣ
	TCHAR							szDescribeString[128];				//������Ϣ
	//�û���ʶ
	DWORD							dwUserID;							//�û���ʶ
	//��������
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	BYTE							cbGender;							//�û��Ա�
	WORD							wFaceID;							//ͷ������
	TCHAR							szMySignature[LEN_MY_SIGNATURE];	//����ǩ��
	//��������
	SCORE                           dwUserDiamond;						//�û�Ԫ��
	DWORD							dwUserMedal;						//�û�����
	DWORD							dwExperience;						//������ֵ
	DWORD							dwLoveLiness;						//�û�����
	//�û�Ȩ��
	BYTE                            cbMasterOrder;                      //����Ա�ȼ� 0��ʾ�ǹ���Ա
	BYTE							cbMemberOrder;						//��Ա�ȼ�   0��ʾ�ǹ���Ա
	//SYSTEMTIME						MemberOverDate;						//��Ա����ʱ��
	//��ϵ��ʽ
	TCHAR                           szIdentityName[LEN_IDENTITY_NAME];  //��ʵ����
	TCHAR                           szIdentityNum[LEN_IDENTITY_NUM];    //���֤��
	TCHAR                           szMobilePhone[LEN_MOBILE_PHONE];    //�ֻ�����	
	//�˺���Ϣ
	TCHAR							szLasLogonIp[LEN_IP_ADDR];			//����¼��ַ
	//SYSTEMTIME						LasLogonDate;						//�������ʱ��
	//����Ȧ 
	DWORD							dwGroupID;							//���ű�ʶ
	TCHAR							szGroupName[LEN_GROUP_NAME];		//��������
	//�Ƹ���Ϣ
	SCORE							lUserScore;							//�û���Ϸ��
	SCORE							lUserGold;							//�û����
	SCORE							lOpenRoomCard;						//�û�����
};

//ƽ̨��¼  
struct STR_SUB_CL_LOGON_PLATFORM
{
	DWORD							dwVersionCheck;						//�汾У��
	TCHAR							szOpenID[33];						//OpenID | tokenID
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	BYTE							cbGender;							//�û��Ա�
	TCHAR							strHeadUrl[LEN_HEAD_URL];			//ͷ���ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};
//ƽ̨��¼
struct STR_CMD_LC_LOGON_PLATFORM
{
	//�������
	DWORD                           dwResultCode;							//�����ʶ
	//������Ϣ
	TCHAR							szDescribeString[LEN_MESSAGE_DESCRIBE];	//������Ϣ
	//�û���ʶ
	DWORD							dwUserID;							//�û���ʶ
	//��������
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szHeadUrl[LEN_HEAD_URL];			//ͷ���ַ
	TCHAR							szMySignature[LEN_MY_SIGNATURE];	//����ǩ��
	//��������
	SCORE                           dwUserDiamond;						//�û�Ԫ��
	DWORD							dwUserMedal;						//�û�����
	DWORD							dwExperience;						//������ֵ
	DWORD							dwLoveLiness;						//�û�����
	//�û�Ȩ��
	BYTE                            cbMasterOrder;                      //����Ա�ȼ� 0��ʾ�ǹ���Ա
	BYTE							cbMemberOrder;						//��Ա�ȼ�   0��ʾ�ǹ���Ա
	//SYSTEMTIME						MemberOverDate;						//��Ա����ʱ��
	//��ϵ��ʽ
	TCHAR                           szIdentityName[LEN_IDENTITY_NAME];  //��ʵ����
	TCHAR                           szIdentityNum[LEN_IDENTITY_NUM];    //���֤��
	TCHAR                           szMobilePhone[LEN_MOBILE_PHONE];    //�ֻ�����	
	//�˺���Ϣ
	TCHAR							szLasLogonIp[LEN_IP_ADDR];			//����¼��ַ
	//SYSTEMTIME						LasLogonDate;					//�������ʱ��
	//����Ȧ 
	DWORD							dwGroupID;							//���ű�ʶ
	TCHAR							szGroupName[LEN_GROUP_NAME];		//��������
	//�Ƹ���Ϣ
	SCORE							lUserScore;							//�û���Ϸ��
	SCORE							lUserGold;							//�û����
	SCORE							lOpenRoomCard;						//�û�����

	//������Ϣ. 
	DWORD							dwOffLineGameID;					//����������GameID, �ǿձ�ʾ�Ƕ�������

	//TODO ���Ӵ���ID
};

//�ظ���¼
struct STR_CMD_LC_LOGON_REPEAT_LOGON
{
	TCHAR szDescribe[LEN_MESSAGE_DESCRIBE];                                              //��ʾ��Ϣ
};

//������ʾ
struct STR_CMD_LC_LOGON_UPDATE_NOTIFY
{
	BYTE							cbUpdateNotify;						//������ʾ		0�汾Ƥ��	1-��������		2-ǿ������
	DWORD							dwCurrentServerVersion;				//��ǰ�������汾
};

//��¼����
struct STR_CMD_LC_LOGON_LOGON_REWARD
{
	BYTE	byRewardType;				//��������
	DWORD	dwRewardCount;				//��������
	TCHAR	szDescribe[LEN_MESSAGE_DESCRIBE];			//����
};

/* *********************************************************************************
**         MAIN:2    MDM_LIST    �б������¼�ɹ����������͸��ͻ��ˣ��ͻ�����������	TODO �ŵ������޸�
** *********************************************************************************/
#define MDM_LIST					2									//�б���Ϣ

//�б���Ϣ����
#define CMD_LC_LIST_TYPE			101									//�����б�TypeItem
#define CMD_LC_LIST_KIND			102									//�����б�KindItem
#define CMD_LC_LIST_NODE			103									//�ڵ��б�NodeItem
#define CMD_LC_LIST_ROOM			104									//�����б�RoomItem

//////////////////////////////////////////////////////////////////////////////////
//���ṹ�塿
//��ȡ����
struct CMD_GP_GetOnline
{
	WORD							wServerCount;						//������Ŀ
	DWORD							wOnLineServerID[MAX_SERVER];		//�����ʶ
};

//��������
struct CMD_GP_KindOnline
{
	WORD							wKindCount;							//������Ŀ
	tagOnLineInfoKind				OnLineInfoKind[MAX_KIND];			//��������
};

//��������
struct CMD_GP_ServerOnline
{
	WORD							wServerCount;						//������Ŀ
	tagOnLineInfoServer				OnLineInfoServer[MAX_SERVER];		//��������
};


/* *********************************************************************************
**      MAIN:3    MDM_SERVICE    ����		//�����������пͻ��˴��������ť��Ϣ
**		˵����1��C(�ͻ���)->L(��½��)����Ϣ�ŷ�Χ��	1-100
**			  2��L(�ͻ���)->C(��½��)����Ϣ�ŷ�Χ��	101-200
**			  3��������Ϣ�ŷ�Χ��	201-300���������������ͣ�
** *********************************************************************************/
//������Ϣ�š�
#define MDM_SERVICE								3							//����

//������Ϣ�š�
#define SUB_CL_SERVICE_USER_FEEDBACK			1							//��ҷ���
#define CMD_LC_SERVICE_USER_FEEDBACK			101							//��ҷ�������

#define SUB_CL_SERVICE_REFRESH_USER_INFO		2							//ˢ���û���Ϣ
#define CMD_CL_SERVICE_REFRESH_USER_INFO		102							//ˢ���û���Ϣ����

#define SUB_CL_SERVICE_QUERY_ROOM_LIST			3							//��ѯ������Ϣ�б�
#define CMD_LC_SERVICE_QUERY_ROOM_LIST			103							//������Ϣ����

#define SUB_CL_SERVICE_MODIFY_PERSONAL_INFO		4							//�޸ĸ������ϣ������û����޸ĵ����϶������ﴦ��
#define CMD_LC_SERVICE_MODIFY_PERSONAL_INFO		104							//�޸ĸ������Ϸ���

#define SUB_CL_SERVICE_GET_RICH_LIST			5							//��ȡ������
#define CMD_LC_SERVICE_GET_RICH_LIST			105							//�����񷵻�

#define SUB_CL_SERVICE_GET_DJ_RECORD_LIST		6							//��ȡ���¼���б�
#define CMD_LC_SERVICE_GET_DJ_RECORD_LIST		106							//��ȡ���¼���б���

#define SUB_CL_SERVICE_GET_XJ_RECORD_LIST		7							//��ȡС��¼���б�
#define CMD_LC_SERVICE_GET_XJ_RECORD_LIST		107							//��ȡС��¼���б���

#define SUB_CL_SERVICE_ONLINE_REWARD			8							//��ȡ���߽��������ս���������ȡ������
#define CMD_LC_SERVICE_ONLINE_REWARD			108							//��ȡ���߽�������

#define SUB_CL_SERVICE_GET_TASK_LIST			9							//��ȡ�����б�			//TODO ����״̬�ĸ����߼���û�޸�
#define CMD_LC_SERVICE_GET_TASK_LIST			109							//��ȡ�����б���

#define SUB_CL_SERVICE_GET_TASK_REWARD			10							//��ȡ������
#define CMD_LC_SERVICE_GET_TASK_REWARD			110							//��ȡ����������

#define SUB_CL_SERVICE_REQUEST_LOTTERY			11							//����齱
#define CMD_LC_SERVICE_REQUEST_LOTTERY			111							//����齱����

#define SUB_CL_SERVICE_GET_RANK_LIST			12							//��ȡָ�����а�			TOOD �����������Ǵ�����Ϸ��ȡ�ģ�С�ֽ��㣬��ֻ�н��յ����ݣ�û�и��µ�����֮���
#define CMD_LC_SERVICE_GET_RANK_LIST			112							//��ȡָ�����а񷵻�

#define SUB_CL_SERVICE_GET_RANK_REWARD			13							//��ȡ���а���
#define CMD_LC_SERVICE_GET_RANK_REWARD			113							//��ȡ���а�������

#define CMD_LC_USER_QUERY_SCORE_INFO			114							//��ѯ���޸ģ��û���ҷ�����ʯ��Ϣ����(�漰����ҷ����ı�Ķ����õ������Ϣ)

//////////////////////////////////////////////////////////////////////////////////
//���ṹ�嶨�塿
//��ҷ���
struct STR_SUB_CL_SERVICE_FEEDBACK
{
	DWORD							dwUserID;								//�û�ID
	TCHAR							szFB_Title[LEN_FEEDBACK_TITLE];			//��������
	TCHAR							szFB_Content[LEN_FEEDBACK_CONTENT];		//��������
	TCHAR							szContact[LEN_MOBILE_PHONE];			//�绰����
};
//��ҷ�������
struct STR_CMD_LC_SERVICE_FEEDBACK
{
	LONG							lResultCode;								//�������
	TCHAR							szDescribeString[LEN_MESSAGE_DESCRIBE];		//��Ϣ����
};

//ˢ���û���Ϣ
struct STR_SUB_CL_SERVICE_REFRESH_INFO
{
	DWORD							dwUserID;							//�û�ID
};
//ˢ���û���Ϣ����
struct STR_CMD_LC_SERVICE_REFRESH_INFO
{
	DWORD                           dwResultCode;                       //�����ʶ
	TCHAR							szDescribeString[128];				//������Ϣ
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	BYTE							cbGender;							//�û��Ա�
	WORD							wFaceID;							//ͷ������
	TCHAR							szMySignature[LEN_MY_SIGNATURE];	//����ǩ��
	SCORE                           dwUserDiamond;						//�û�Ԫ��
	DWORD							dwUserMedal;						//�û�����
	DWORD							dwExperience;						//������ֵ
	DWORD							dwLoveLiness;						//�û�����
	BYTE                            cbMasterOrder;                      //����Ա�ȼ� 0��ʾ�ǹ���Ա
	BYTE							cbMemberOrder;						//��Ա�ȼ�   0��ʾ�ǹ���Ա
	//SYSTEMTIME					MemberOverDate;						//��Ա����ʱ��
	TCHAR                           szIdentityName[LEN_IDENTITY_NAME];  //��ʵ����
	TCHAR                           szIdentityNum[LEN_IDENTITY_NUM];    //���֤��
	TCHAR                           szMobilePhone[LEN_MOBILE_PHONE];    //�ֻ�����
	TCHAR							szLasLogonIp[LEN_IP_ADDR];			//����¼��ַ
	//SYSTEMTIME						LasLogonDate;						//�������ʱ��		TODO �ͻ���û���������ʱ�䣬У���ǲ��ǵ������ݰ���С
	DWORD							dwGroupID;							//���ű�ʶ
	TCHAR							szGroupName[LEN_GROUP_NAME];		//��������
	SCORE							lUserScore;							//�û�����
	SCORE							lUserGold;							//�û����
	SCORE							lUserRoomCard;						//�û�����
};

//������Ϣ�б�
struct STR_SUB_CL_SERVICE_QUERY_ROOMLIST
{
	DWORD dwUserID;
};
//������Ϣ�б���
struct STR_CMD_LC_SERVICE_QUERY_ROOMLIST
{
	DWORD dwTableID;			//����ID
	DWORD dwPassword;			//��������
	DWORD dwCreaterID;			//�������ID
	WORD  wJuShu;				//����
	BYTE  byMode;				//ģʽ
	BYTE  byZhuangType;			//ׯ����
	BYTE  byPlayerCount;		//�����
	BYTE  byMaxPlayerCount;		//��������
	BYTE  byIsStart;			//�Ƿ�ʼ
	BYTE  byCost;				//�ۿ���
	TCHAR szTime[24];			//����ʱ��
	DWORD dwUserID[5];			//���ID
	TCHAR szNickName[5][LEN_NICKNAME];		//����ǳ�
	DWORD dwScore[5];					//�û�����
};

//�޸ĸ�������	
struct STR_SUB_CL_SERVICE_MODIFY_PERSONAL_INFO
{
	//�û���ʶ
	DWORD							dwUserID;							//�û�ID
	TCHAR							szOldLogonPassword[LEN_PASSWORD];	//�ɵĵ�½����
	//������Ϣ
	TCHAR							szNewLogonPassword[LEN_PASSWORD];	//�µĵ�½���루�����޸ģ��;ɵ�һ����
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	BYTE							cbGender;							//�û��Ա�	
	TCHAR							szHeadImageUrl[LEN_HEAD_URL];			//ͷ���ַ
	TCHAR							szSignature[LEN_MY_SIGNATURE];		//����ǩ��
	//��ϵ��ʽ
	TCHAR							szRealName[LEN_IDENTITY_NAME];		//��ʵ����
	TCHAR							szIDCardNum[LEN_IDENTITY_NUM];		//���֤��
	TCHAR							szPhoneNum[LEN_MOBILE_PHONE];		//���ֻ���
	//��������
	DWORD							dwProxyUserID;						//�����û�ID		//TODO ����ID�����ݿ�����ʱδ���ӣ���������
};
//�޸ĸ������Ϸ���
struct STR_CMD_LC_SERVICE_MODIFY_PERSONL_INFO
{
	LONG							lResultCode;						//��������
	TCHAR							szDescribeString[128];				//�ɹ���Ϣ
};

//��ȡ������
struct STR_SUB_CL_SERVICE_GET_RICHLIST
{

};
//����������ṹ��
struct tagSingleRichInfo
{
	TCHAR szName[LEN_NICKNAME];				//�û���
	DWORD dwMoney;							//��Ǯ
	TCHAR szWechatAccount[LEN_ACCOUNTS];	//΢���˺�
	DWORD dwRegistID;						//��֤ID
	TCHAR szQQ[LEN_ACCOUNTS];				//QQ��
	TCHAR szMobilePhone[LEN_MOBILE_PHONE];	//�ֻ���
};
//��ȡ�����񷵻�
struct STR_CMD_LC_SERVICE_GET_RICHLIST
{
	BYTE byCount;						//���а�����
	tagSingleRichInfo RegalInfoList[15];	//�����б�
};

//��ȡ�û�¼���б�
struct STR_SUB_CL_SERVICE_GET_DJ_RECORDLIST
{
	DWORD dwUserID;
};
//��ȡ�û�¼���б���
struct STR_CMD_LC_SERVICE_GET_DJ_RECORDLIST
{
	DWORD							dwTableID;			//����ID
	DWORD							dwUserID[5];		//���ID
	TCHAR							szUserName[5][LEN_NICKNAME];//�û��ǳ�
	BYTE							byGameMode;			//��Ϸģʽ
	BYTE							byZhuangType;		//��ׯ����
	WORD							wCurrentJuShu;		//��ǰ����
	WORD							wTotalJuShu;		//�ܾ���
	TCHAR							szGameTime[30];		//��Ϸʱ��
	TCHAR							szTotalScore[50];	//��Ҵ���ܷ���
};

//��ȡС��¼���б�
struct STR_SUB_CL_SERVICE_GET_XJ_RECORDLIST
{
	DWORD dwTableID;												//����ID
};
//��ȡС��¼���б���
struct STR_CMD_LC_SERVICE_GET_XJ_RECORDLIST
{
	WORD							wCurrentJuShu;					//��ǰ����
	TCHAR							szNickName[5][LEN_NICKNAME];	//�û��ǳ�
	TCHAR							szScore[50];					//���С�ַ���

	//TODO ��������Ӿ��������
};

//��ȡ���߽���
struct STR_SUB_CL_SERVICE_ONLINE_REWARD   
{
	DWORD dwUserID;
};
//��ȡ���߽�������
struct STR_CMD_LC_SERVICE_ONLINE_REWARD
{
	BYTE  byType;		//��������
	DWORD dwCount;		//��������
};

//��ȡ�����б�
struct STR_SUB_CL_SERVICE_GET_TASKLIST
{
	DWORD	dwUserID;				//�û�ID
};
//������Ϣ
struct tagTaskInfo
{
	WORD	wTaskID;							//����ID
	BYTE	cbTaskType;							//�������� 1�ճ�; 2�ܳ�; 3�³�; 4�ɾ�
	BYTE	cbActionType;						//���������� ��1����Ϣ; 2΢�ŷ���; 3��ֵ; 4������Ϸ; 5�����Ϸ	
	DWORD	dwNeedCount;						//����������������, �������5�ַ�����Ϸ, �������5	
	TCHAR	szDescribe[128];					//��������
	BYTE	byRewardType;						//��������1���; 2����; 3��ʯ
	DWORD	dwRewardCount;						//��������
	BYTE	cbTaskStatus;						//����״̬ ��1-������  1-��ɵ�δ��ȡ 2-�������ȡ
	DWORD	dwFinishedCount;					//����ɵ�������������
};
//��ȡ�����б���
struct STR_CMD_LC_SERVICE_GET_TASKLIST
{
	WORD	wTaskID;							//����ID
	BYTE	cbTaskType;							//�������� 1�ճ�; 2�ܳ�; 3�³�; 4�ɾ�
	BYTE	cbActionType;						//���������� ��1����Ϣ; 2΢�ŷ���; 3��ֵ; 4������Ϸ; 5�����Ϸ	
	DWORD	dwNeedCount;						//����������������, �������5�ַ�����Ϸ, �������5	
	TCHAR	szDescribe[128];					//��������
	BYTE	byRewardType;						//��������1���; 2����; 3��ʯ
	DWORD	dwRewardCount;						//��������
	BYTE	cbTaskStatus;						//����״̬ ��0-������  1-��ɵ�δ��ȡ 2-�������ȡ
	DWORD	dwFinishedCount;					//����ɵ���������
	//������Ϣ
	BYTE	cbListFinishMask;					//�����б�����ɱ�־ 0-δ��� 1-���
};

//��ȡ������
struct STR_SUB_CL_SERVICE_GET_TASK_REWARD
{
	DWORD	dwUserID;
	WORD	wTaskID;	
};
//��ȡ����������
struct STR_CMD_LC_SERVICE_GET_TASK_REWARD				
{
	LONG	lResultCode;												//0-�������  ����-����δ���
	TCHAR	szDescribe[128];											//������������� �����,���10000���
};

//��ȡ���а��б�
struct STR_SUB_CL_SERVICE_GET_RANKLIST
{
	BYTE	byIndex;  //�����,0�������ķ��� 1����ʤ�� 2�������� 3����ʤ�� 4�������� 5����ʤ�� 6�������� 7����ʤ��
};
//���а���Ϣ
struct tagRankInfo
{
	BYTE	byIndex;				//�����
	DWORD	dwUserID;				//�û�ID
	DWORD	dwCount;				//���ѻ�ʤ������
	BYTE	byRankNum;				//����
	BYTE	byReceived;				//����ȡ
	TCHAR	szNickName[LEN_NICKNAME];//�ǳ�
	TCHAR	szHeadUrl[LEN_HEAD_URL];	//ͷ��
};
//��ȡ���а��б���
struct STR_CMD_LC_SERVICE_GET_RANKLIST
{
	tagRankInfo rankItemInfo;
};

//����齱
struct STR_SUB_CL_SERVICE_REQUEST_LOTTERY
{
	DWORD	dwUserID;				//�û�ID
};
//����齱����
struct STR_CMD_LC_SERVICE_REQUEST_LOTTERY
{
	LONG							lResultCode;						//��������
	TCHAR							szDescribeString[128];				//������Ϣ
	BYTE							byIndex;							//ҡ����ţ�ת�̵ķ�����������0-11	
};

//��ȡ���а���
struct STR_SUB_CL_SERVICE_GET_RANK_REWARD
{
	BYTE	byIndex;
	DWORD	dwUserID;
};
//��ȡ���а�������   
struct STR_CMD_LC_SERVICE_GET_RANK_REWARD				
{
	LONG	lResultCode;												//0-��ȡ�ɹ�  ����ʧ��
	TCHAR	szDescribe[128];											//������������� ��ȡ�ɹ�,���10000���
};

//��ѯ�û���ҷ�����Ϣ		TODO ��¼���б�ʱ�õ���ò�ƴ���	�����޸�Ϊ�ͻ��˴�����ˢ�°�ť
struct CMD_GP_QUERY_ScoreInfo
{
	DWORD							dwUserID;							//�û�ID
};
//�����û���ҷ�����Ϣ
struct STR_CMD_LC_QUERY_SCORE_INFO
{
	LONG							lResultCode;						//��������
	TCHAR							szDescribeString[128];				//������Ϣ
	DWORD							dwUserID;							//�û�ID
	LONGLONG						lOpenRoomCard;						//����
	LONGLONG						lGold;								//���
	LONGLONG						lDiamond;							//��ʯ
};


/* *******************************************************************************
**          MAIN:4      MDM_GROUP   ����Ȧ
**			˵����1��C(�ͻ���)->L(��½��)����Ϣ�ŷ�Χ��	1-100
**				  2��L(�ͻ���)->C(��½��)����Ϣ�ŷ�Χ��	101-200
**				  3��������Ϣ�ŷ�Χ��	201-300
** ******************************************************************************/
#define MDM_GROUP						4									//����Ȧ��Ϣ

#define SUB_CL_GROUP_CREATE_GROUP		1									//��������Ȧ
#define CMD_LC_GROUP_CREATE_GROUP		101									//��������Ȧ�ɹ����أ�ͬʱ�������û�����Ȧ�б�

#define SUB_CL_GROUP_JOIN_GROUP			2									//�����������Ȧ
#define CMD_LC_GROUP_JOIN_GROUP			102									//�����������Ȧ����

#define SUB_CL_GROUP_DISMISS_GROUP		3									//��ɢ����Ȧ
#define CMD_LC_GROUP_DISMISS_GROUP		103									//��ɢ����Ȧ����

#define SUB_CL_GROUP_LEAVE_GROUP		4									//�˳�����Ȧ		
#define CMD_LC_GROUP_LEAVE_GROUP		104									//�˳�����Ȧ����

#define SUB_CL_GROUP_SET_ROOMCARD		5									//��������Ȧ������ͬʱ��������Ϣ��3�û���ҷ�����Ϣ116������Ȧ�б���Ϣ105��
#define CMD_LC_GROUP_SET_ROOMCARD		105									//��������Ȧ��������

#define SUB_CL_GROUP_GET_GROUP_LIST		6									//��ȡ�û�����Ȧ�б�(ֻ�л�����Ϣ)
#define CMD_LC_GROUP_GET_GROUP_LIST		106									//��ȡ�û�����Ȧ�б��أ����ض�θ���Ϣ��ÿ�η���һ������Ȧ�Ļ�����Ϣ��

#define SUB_CL_GROUP_GET_MEMBER_LIST	7									//��ȡָ������Ȧ�ĳ�Ա�б�
#define CMD_LC_GROUP_GET_MEMBER_LIST	107									//��ȡָ������Ȧ�ĳ�Ա�б���

#define SUB_CL_GROUP_ALLOW_USER_JOIN	8									//ͬ���������Ȧ��ͬʱˢ������Ȧ�б�
#define CMD_LC_GROUP_ALLOW_USER_JOIN	108									//ͬ���������Ȧ����

#define SUB_CL_GROUP_REFUSE_USER_JOIN	9									//�ܾ���������Ȧ
#define CMD_LC_GROUP_REFUSE_USER_JOIN	109									//�ܾ���������Ȧ����

#define SUB_CL_GROUP_GET_APPLY_LIST		10									//��ȡָ������Ȧ ��������û��б�
#define CMD_LC_GROUP_GET_APPLY_LIST		110									//��ȡָ������Ȧ ��������û��б���

#define SUB_CL_GROUP_SEND_MSG			11									//��������Ȧ��Ϣ��Ⱥ�ģ�		TODO �����������û��Ƿ��յõ���Ϣ
#define CMD_LC_GROUP_SEND_MSG			111									//��������Ȧ��Ϣ����

#define SUB_CL_GROUP_SET_NOTICE			12									//��������Ȧ���棬���ػ������Ȧ��Ϣ�����г�Ա��
//#define CMD_LC_GROUP_SET_NOTICE		112									//��������Ȧ���淵��

#define SUB_CL_GROUP_GET_GROUP_INFO		13									//���ָ������Ȧ������Ϣ		TODO �����������û��Ƿ����óɹ�
#define CMD_LC_GROUP_GET_GROUP_INFO		113									//���ָ������Ȧ������Ϣ����

#define SUB_CL_GROUP_KICK_USER			14									//�߳���Ա
#define CMD_LC_GROUP_KICK_USER			114									//�߳���Ա����

//////////////////////////////////////////////////////////////////////////////////

//��������Ȧ
struct STR_SUB_CL_GROUP_CREATE_GROUP
{
	TCHAR	szGroupName[LEN_GROUP_NAME];	//����Ȧ����
	DWORD	dwOwnerID;						//Ȧ��ID
	BYTE	GameId;							//��ϷID��5-�����齫   4-������ţ
};
//��������Ȧ����
struct STR_CMD_LC_GROUP_CREATE_GROUP
{
	LONG	lResultCode;								//��������
	TCHAR	szDescribeString[LEN_MESSAGE_DESCRIBE];		//������Ϣ
	DWORD	dwGroupID;									//����ȦID
	TCHAR	szGroupName[LEN_GROUP_NAME];				//����Ȧ����
	WORD	wCurrentUserCount;							//��ǰ�û���
	WORD	wMaxUserCount;								//����û���
};

//�����������Ȧ
struct STR_SUB_CL_GROUP_JOIN_GROUP
{
	DWORD	dwGroupID;
	DWORD	dwUserID;
};
struct tagJoinGroup
{
	DWORD	dwUserID;	
	TCHAR	szUserName[LEN_NICKNAME];
	DWORD	dwGroupID;
	TCHAR	szHeadUrl[LEN_HEAD_URL];
};
//�����������Ȧ����
struct STR_CMD_LC_GROUP_JOIN_GROUP
{
	LONG			lResultCode;								//��������
	TCHAR			szDescribeString[LEN_MESSAGE_DESCRIBE];		//������Ϣ
	tagJoinGroup	applyUserInfo;								//��������Ϣ
};

//��ɢ����Ȧ
struct STR_SUB_CL_GROUP_DISMISS_GROUP
{
	DWORD	dwUserID;				//��ɢ����Ȧ���û�ID
	DWORD	dwGroupID;				//����ȦID
};
//��ɢ����Ȧ����
struct STR_CMD_LC_GROUP_DISMISS_GROUP
{
	LONG						lResultCode;								//�������
	TCHAR						szDescribeString[LEN_MESSAGE_DESCRIBE];		//������Ϣ
	DWORD						dwGroupID;									//��ɢ����ȦID
};

//�˳�����Ȧ
struct STR_SUB_CL_GROUP_LEAVE_GROUP
{
	DWORD				dwUserID;							//�û�ID
	DWORD				dwGroupID;							//����ȦID

};
//�˳�����Ȧ����
struct STR_CMD_LC_GROUP_LEAVE_GROUP
{
	LONG				lResultCode;								//�������
	TCHAR				szDescribeString[LEN_MESSAGE_DESCRIBE];		//������Ϣ
	DWORD				dwGroupID;									//����ȦID
};

//��������Ȧ����
struct STR_SUB_CL_GROUP_SET_ROOMCARD
{
	DWORD	dwGroupID;	//����ȦID
	DWORD	dwUserID;	//�û�ID
	BYTE	cbPutIn;	//ת��0  ȡ��1
	DWORD	dwRoomCard;	//������
};
//��������Ȧ��������
struct STR_CMD_LC_GROUP_SET_ROOMCARD
{
	LONG					lResultCode;								//�������
	TCHAR					szDescribeString[LEN_MESSAGE_DESCRIBE];		//������Ϣ
	DWORD					dwGroupRoomCard;							//����Ȧ������
};

//��ȡ�û�����Ȧ�б�
struct STR_SUB_CL_GROUP_GET_GROUP_LIST
{
	DWORD dwUserID;							//�û�ID
};
//��ȡ�û�����Ȧ�б���
struct STR_CMD_LC_GROUP_GET_GROUP_LIST
{
	DWORD dwGroupID;						//����ȦID
	TCHAR szGroupName[LEN_GROUP_NAME];		//����Ȧ����
	WORD  wCurrentUserCount;				//����Ȧ��ǰ����
	WORD  wMaxUserCount;					//����Ȧ�������
	DWORD dwOwnerID;						//Ȧ��ID
	TCHAR szOwnerHeadUrl[LEN_HEAD_URL];		//Ȧ��ͷ��		
};

//��ȡָ������Ȧ��Ա�б�
struct STR_SUB_CL_GROUP_GET_MEMBER_LIST
{
	DWORD	dwGroupID;
};
//��ȡָ������Ȧ��Ա�б���
struct STR_CMD_LC_GROUP_GET_MEMBER_LIST
{
	DWORD dwUserID;							//��ԱID
	DWORD dwGroupID;						//����ȦID
	TCHAR szNickName[LEN_NICKNAME];			//��Ա�ǳ�
	TCHAR szHeadUrl[LEN_HEAD_URL];			//��Աͷ��
};

//ͬ���û���������Ȧ
struct STR_SUB_CL_GROUP_ALLOW_USER_JOIN
{
	DWORD	dwJoinUserID;		//ͬ�������û�ID
	DWORD	dwGroupID;			//����ȦID
};
//ͬ���û���������Ȧ����
struct STR_CMD_LC_GROUP_ALLOW_USER_JOIN
{
	LONG							lResultCode;								//�������
	TCHAR							szDescribeString[LEN_MESSAGE_DESCRIBE];		//������Ϣ
	DWORD							dwJoinUserID;								//ͬ�������û�ID
	DWORD							dwGroupID;									//����ȦID
};

//�ܾ��û���������Ȧ
struct STR_SUB_CL_GROUP_REFUSE_USER_JOIN
{
	DWORD	dwRefuseUserID;								//�ܾ�������û�ID
	DWORD	dwGroupID;									//����ȦID
};
//�ܾ��û���������Ȧ����
struct STR_CMD_LC_GROUP_REFUSE_USER_JOIN
{
	TCHAR	szDescribeString[LEN_MESSAGE_DESCRIBE];		//������Ϣ
};

//��ȡָ������Ȧ����������û��б�
struct STR_SUB_CL_GROUP_GET_APPLY_LIST
{
	DWORD	dwUserID;			//�����û�ID
	DWORD	dwGroupID;			//����ȦID
};
//�����������Ȧ����
struct STR_CMD_LC_GROUP_GET_APPLY_LIST
{
	LONG			lResultCode;								//��������
	TCHAR			szDescribeString[LEN_MESSAGE_DESCRIBE];		//������Ϣ
	tagJoinGroup	applyUserInfo;								//��������Ϣ
};

//��������Ȧ��Ϣ
struct STR_SUB_CL_GROUP_SEND_MSG
{
	DWORD dwSendUserID;					//������Ϣ�û�ID
	TCHAR szSendUserNick[LEN_NICKNAME];	//������Ϣ�û��ǳ�
	DWORD dwGroupID;					//����ȦID
	BYTE  wMsgID;						//��ʾ�Զ�����Ϣ
	TCHAR szMsg[LEN_GROUP_MESSAGE];		//�Զ�����Ϣ����
};
//��������Ȧ������Ϣ
struct STR_CMD_LC_GROUP_SEND_MSG
{
	DWORD dwSendUserID;					//������Ϣ�û�ID
	TCHAR szSendUserNick[LEN_NICKNAME];	//������Ϣ�û��ǳ�
	DWORD dwGroupID;					//����ȦID
	BYTE  wMsgID;						//��ʾ�Զ�����Ϣ
	TCHAR szMsg[LEN_GROUP_MESSAGE];		//�Զ�����Ϣ����
};

//��������Ȧ����
struct STR_SUB_CL_GROUP_SET_GROUP_NOTICE
{
	DWORD   dwUserID;					//�û�ID
	DWORD	dwGroupID;					//����ȦID
	TCHAR	szNotice[LEN_GROUP_NOTICE];				//�Թ�������
};
//��������Ȧ���淵��
struct STR_CMD_LC_GROUP_SET_GROUP_NOTICE
{
	LONG			lResultCode;								//�������
	TCHAR			szDescribeString[LEN_MESSAGE_DESCRIBE];		//������Ϣ
};

//���ָ������Ȧ��Ϣ
struct STR_SUB_CL_GROUP_GET_GROUP_INFO
{
	DWORD   dwGroupID;						//����ȦID
};
//���ָ������Ȧ��Ϣ����
struct STR_CMD_LC_GROUP_GET_GROUP_INFO
{
	DWORD dwGroupID;						//����ȦID
	TCHAR szGroupName[LEN_GROUP_NAME];		//����Ȧ����
	DWORD dwOwnerID;						//Ȧ��ID
	TCHAR szOwnerHeadUrl[LEN_HEAD_URL];		//Ȧ��ͷ��
	WORD  wCurrentUserCount;				//����Ȧ��ǰ����
	WORD  wMaxUserCount;					//����Ȧ�������
	DWORD dwRoomCard;						//����Ȧ����
	TCHAR szNotice[LEN_GROUP_NOTICE];		//����Ȧ����
};

//�߳��û�  
struct STR_SUB_CL_GROUP_KICK_USER
{
	DWORD dwOperateUserID;		//���˵��û�ID
	DWORD dwKickUserID;			//���ߵ��û�ID
	DWORD dwGroupID;			//����ȦID
};
//�߳��û�����
struct STR_CMD_LC_GROUP_KICK_USER
{
	LONG							lResultCode;								//�������
	TCHAR							szDescribeString[LEN_MESSAGE_DESCRIBE];		//������Ϣ
};

/* *******************************************************************************
**          MAIN:5      MDM_OTHERS   ������Ϣ
** ******************************************************************************/
//������Ϣ�š�
#define MDM_OTHERS								5							//������Ϣ

//������Ϣ�š�
#define SUB_CL_OTHERS_GIVE_PROPS				1							//���͵���  //TODO ���ص��ǵ�½����311		TODO�޸�����
#define CMD_LC_OTHERS_GIVE_PROPS				101							//���͵��߷���

#define SUB_CL_OTHERS_RECHARGE_INFO				2							//��ֵ��Ϣ
#define CMD_LC_OTHERS_RECHARGE_INFO				102							//���͵��߷���

#define SUB_CL_OTHERS_EXCHANGE_INFO				3							//�һ���Ϣ��Ǯ->���ߣ�
#define CMD_LC_OTHERS_EXCHANGE_INFO				3							//�һ���Ϣ����

//////////////////////////////////////////////////////////////////////////////////
//���͵���
struct SUB_CL_OTHER_GIVE_PROPS  
{
	DWORD dwSrcUserID;   //�۳��û�ID
	DWORD dwDstUserID;	//Ŀ���û�ID
	BYTE  byType;		//��������
	DWORD dwCount;		//��������
};

//���͵��߷��ص������߽��� TODO �޸ķ�������

//��ֵ��Ϣ
struct STR_SUB_CL_OTHER_RECHARGE_INFO
{
	DWORD								dwUserID;							//�û� I D
};
//��ֵ��Ϣ����
struct STR_CMD_LC_OTHER_RECHARGE_INFO
{
	SCORE								dwMinMoney;		//���ٳ�ֵǮ  
	UINT32								dwChangeScale;  //�һ�����
};

//�һ�����
struct STR_SUB_CL_OTHER_EXCHANGE_INFO
{
	DWORD								dwUserID;							//�û� I D
};
//�һ����߷���
struct STR_CMD_LC_OTHER_EXCHANGE_INFO
{
	SCORE								dwMinMoney;		///���ٳ�ֵǮ  
	UINT32								dwChangeScale;  ///�һ�����
	SCORE								dwWithdrawals;  ///������RMB
	SCORE								dwBankMoney;	///���н��
};

//���ò������
struct STR_CMD_LC_OTHER_OPERATE_RESULT
{
	LONG							lResultCode;						//�������
};
//////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif