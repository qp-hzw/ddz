#ifndef SUB_DEFINE
#define SUB_DEFINE


//��ׯģʽ
#define ROOMRULE_OPTION_ROBTYPE_FREE		0								//������ׯ
#define ROOMRULE_OPTION_ROBTYPE_JIAOSANFEN	1								//������


//�÷�ģʽ
#define GAME_SCORE_NUM						2								//�÷�ģʽ������Ŀ
#define GAME_SCORE_MODE_CLASSIC				0								//����ģʽ
#define GAME_SCORE_MODE_CRAZY				1								//���ӱ�ģʽ

//��������
#define TABLE_MODE_FANG_KA					0								//����ģʽ
#define TABLE_MODE_JJC						1								//������ģʽ
#define TABLE_MODE_GOLD						2								//���ģʽ

//����׷�
#define ROOMRULE_MIN_CELL_SCORE				1								//������С�׷�


//�û�����
#define MAX_CARD_COUNT						42								//��������˿���Ŀ
#define MAX_GAME_SCORE_TYPES				50								//�����Ϸ���Ʊ������࣬������50

//��Ч����
#define INVALID_CARD						0								//��Ч����

//////////////////////////////////////////////////////////////////////////
//��Ϸ״̬�������������أ�
#define GS_WK_FREE				    GAME_STATUS_FREE					//��Ϸ״̬-�ȴ���ʼ
#define GS_WK_ROB					GAME_STATUS_PLAY+1					//��Ϸ״̬-��ׯ
#define GS_WK_ADD_SCORE				GAME_STATUS_PLAY+2					//��Ϸ״̬-��ׯ
#define GS_WK_ADDSCORE				GAME_STATUS_PLAY+3					//��Ϸ״̬-��ע
#define GS_WK_JIAO_FEN				GAME_STATUS_PLAY+7					//�з�״̬

#define GS_WK_FANZHU				GAME_STATUS_PLAY+3					//��Ϸ״̬-����
#define GS_WK_KOUDI					GAME_STATUS_PLAY+4					//��Ϸ״̬-�۵ף��ǵ��ƣ�
#define GS_WK_OUT_CARD				GAME_STATUS_PLAY+5					//��Ϸ״̬-����
#define GS_WK_XJ_GAMEEND			GAME_STATUS_PLAY+6					//��Ϸ״̬-С�ֽ���
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//��ʱ��   --  1.��Ϸ״̬ 2.��Ϣ�� 3.��ʱ��  Ӧ���ں����б���ͳһ
#define	IDI_ROB_BANKER					1							//��ׯ��ʱ��
#define IDI_ADD_SCORE					2							//��ע��ʱ��
#define IDI_MING_PAI					3							//���ƶ�ʱ��
#define IDI_ADD_SCORE_ROBOT				4							//��������ע��ʱ������ʱ��

#define IDI_KOU_DI						4							//�۵׶�ʱ��
#define IDI_OUT_CARD					5							//���ƶ�ʱ��
#define IDI_XJGAME_USER_READY			6							//С����Ϸ�����û�׼����ʼ��ʱ��
#define IDI_FAN_ZHU_DELAY				7							//�����ӳٶ�ʱ�����ͻ��˴����귢�ƶ���֮���ٸ���һ���˷��ͷ�����ʾ
#define IDI_ROUND_END_DELAY				8							//һ�ֽ����ӳٶ�ʱ�����ͻ��˴�����ֽ��㶯��
#define IDI_ROB_JiaoFen					9							//�зֶ�ʱ��

//������ʱ������ʱ��
#define IDI_TIME_ROB_BANKER				10000						//��ׯ��ʱ������ʱ��
#define IDI_TIME_ADD_SCORE				5000						//��ע��ʱ������ʱ��
#define IDI_TIME_MING_PAI				3000						//���ƶ�ʱ������ʱ��
#define IDI_TIME_ADD_SCORE_ROBOT		1000						//��������ע��ʱ������ʱ��

#define IDI_TIME_KOU_DI					20000						//�۵׶�ʱ������ʱ��
#define IDI_TIME_OUT_CARD				20000						//���ƶ�ʱ������ʱ��
#define IDI_TIME_XJGAME_USER_READY		8000						//С�ֽ����û�׼����ʱ������ʱ��
#define IDI_TIME_FAN_ZHU_DELAY			6000						//�����ӳٶ�ʱ������ʱ��
#define IDI_TIME_ROUND_END_DELAY		2000						//һ�ֽ����ӳٶ�ʱ������ʱ��
#define IDI_TIME_ROB_JiaoFen			10000						//�зֶ�ʱ��
///////////////////////////////

///////////////////////////////
//�����Ϸ����
#define GAME_CONCLUDE_CONTINUE			0xFF						//��ֽ���������
#define GAME_CONCLUDE_NORMAL			0xFE						//���ֱ�ӽ���
///////////////////////////////

//////////////////////////////////////////////////////////////////////////
//�����ϵ��û�״̬
#define USER_NULL                0         //�û�״̬-���ӿ�--û����
#define USER_STANDUP             1         //�û�״̬-վ��  --�Թ���   --���뷿���Ĭ������
#define USER_SITDOWN             2         //�û�״̬-����  --��Ϸ��� --��������Ϸ���û�
#define USER_ONREADY             3         //�û�״̬-׼��             --��׼����, �������Կ�ʼ��Ϸ  //TODONOW �Ƿ���Ҫ���Ӷ�ʱ���������ӷ������˹���???
#define USER_PLAYING             4         //�û�״̬-��Ϸ��           --�����㿪ʼ��,����׼��������ҽ��뿪ʼ״̬    
//////////////////////////////////////////////////////////////////////////
//����������
#define	ROB_TYPE_CALL			1			//�е���
#define	ROB_TYPE_ROB			2			//������
#define	ROB_STATE_NON_RESPONE	0			//��δ��ʼ��ׯ
#define	ROB_STATE_PASS			1			//��
#define	ROB_STATE_AGREE			2			//��/��

#define JIAOFEN_START			15			//��һ����ҽз�  1111
#define JIAOFEN_3				1<<3		//�з�����
#define JIAOFEN_2				1<<2		//�зֶ���
#define JIAOFEN_1				1<<1		//�з�һ��

#define	MING_PAI_TYPE_OUTCARD	1			//��������
#define	MING_PAI_TYPE_GAMESTART	2			//��Ϸ��ʼ����
#define	MING_PAI_TYPE_DEAL		3			//��������

#define OUT_CARD_FAIL			0									//����ʧ��
#define OUT_CARD_SUCCESS		1									//���Ƴɹ�

#define OUT_CARD_PASS			1									//���ƹ���Ҫ����
#define OUT_CARD_NORMAL			0									//��������

#define TURN_END				0									//һ�ֽ���
#define TURN_NOT_END			1									//һ��δ����

#define FULL_COUNT				54									//һ��ȫ����Ŀ

#define MAX_WEAVE_NUM			21									//����������ȫ��Ϊ���ţ�
#define MAX_BOOM_NUM			5									//���ը������ȫ��Ϊը����

#define HARD_BOOM_TIMES			4									//���ը������ȫ��Ϊը����
#define RUAN_BOOM_TIMES			2									//���ը������ȫ��Ϊը����

//��������
#define ST_ORDER				0									//��С����
#define ST_COUNT				1									//��Ŀ����

//��������
#define	DEFALUT_UPGRADE_SCORE	80									//Ĭ�϶��ٷ�����
#define	COLOR_RIGHT				40									//��ɫȨλ
#define LEN_SORT_RIGHT			5									//Ȩλ�����С
#define VALUE_ERROR				0x00								//��Ч������ֵ����
#define COLOR_ERROR				0xFF								//��Ч��ɫ����
#define BIG_JOKER				0x4F								//����
#define SMALL_JOKER				0x4E								//С��
#define NORMAL_LEAVE_CARD_NUM	3									//�����۵׿�����
#define NORMAL_HAND_CARD_NUM	17									//����������
#define	MAX_LEAVE_CARD_NUM		3									//���۵׿�����
#define MAX_COLOR_TYPE_NUM		4									//��ɫ����������ȥ��С��
#define MAX_FANZHU_TYPE_NUM		6									//���ķ�����������4�Լ��� + ��С����
#define LEN_MAX_INDEX			67									//���������0-12�� 13-25÷�� 26-38���� 39-51���� 52-53С��������
#define SMALL_JOKER_INDEX		65									//С������
#define BIG_JOKER_INDEX			66									//��������
#define MAX_SCORE_CARD_NUM		24									//���÷ֿ����������� 5 10 K��
#define START_LEVEL_VALUE		2									//��ʼ���ƴ�С

#define COLOR_MAIN_CARD			0x04								//���Ʊ�־��������С���ͼ��ƣ�
#define COLOR_HEI_TAO			0x03								//����
#define COLOR_HONG_TAO			0x02								//����
#define COLOR_MEI_HUA			0x01								//÷��
#define COLOR_FANG_KUAI			0x00								//����

//������
#define CT_ERROR					0									//��������
#define CT_SINGLE					1									//��������
#define CT_DOUBLE					2									//��������
#define CT_THREE					3									//��������
#define CT_SINGLE_LINE				4									//��������
#define CT_DOUBLE_LINE				5									//��������
#define CT_THREE_LINE				6									//��������
#define CT_THREE_LINE_TAKE_ONE		7									//����һ��
#define CT_THREE_LINE_TAKE_TWO		8									//����һ��
#define CT_FOUR_LINE_TAKE_ONE		9									//�Ĵ�����
#define CT_FOUR_LINE_TAKE_TWO		10									//�Ĵ�����
#define CT_RUAN_BOMB				11									//��ը��
#define CT_BOMB_CARD				12									//ը������
#define CT_LAIZI_BOMB				13									//����ը��
#define CT_MISSILE_CARD				14									//�������

//����
#define CT_SAME_2				2									//��������
#define CT_TRACKOR_2			4									//2��������
#define CT_TRACKOR_3			6									//3��������
#define CT_TRACKOR_4			8									//4��������
#define CT_TRACKOR_5			10									//5��������
#define CT_TRACKOR_6			12									//6��������
#define CT_TRACKOR_7			14									//7��������
#define CT_TRACKOR_8			16									//8��������
#define CT_TRACKOR_9			18									//9��������
#define CT_TRACKOR_10			20									//10��������
#define CT_TRACKOR_11			22									//11��������
#define CT_TRACKOR_12			24									//12��������
#define CT_TRACKOR_13			26									//13��������


#define CT_THROW_CARD			15									//˦������

#define MAX_PACK				4									//�����		TODO ��֪�������ɶ�ˣ�����
#define MAX_TRACKOR				18									//�����������
#define INDEX_OUT				0									//��������
#define INDEX_HAND				1									//��������
#define INDEX_FIRST				2									//�׳�����

//��������
#define	SCORE_DA_GUANG			0									//������
#define	SCORE_XIAO_GUANG		40									//С�����
#define	SCORE_CHANGE_BANKER		80									//��ׯ����
#define	SCORE_UPGRADE			120									//��������
#define	SCORE_UPGRADE_ONE		40									//��һ����Ҫ�÷���

//��������
#define	LEVEL_DA_GUANG			3									//������
#define	LEVEL_XIAO_GUANG		2									//С�����
#define	LEVEL_CHANGE_BANKER		1									//��ׯ����
#define	LEVEL_UPGRADE_ONE		40									//��һ����Ҫ�÷���

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//������Ϣ
struct tagSameDataInfo
{
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbBlockCount;						//�ƶ���Ŀ
	BYTE							cbSameData[MAX_CARD_COUNT];			//��������
};

//��������Ϣ
struct tagTractorDataInfo
{
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbTractorCount;						//���Ƹ���
	BYTE							cbTractorMaxIndex;					//����λ��
	BYTE							cbTractorMaxLength;					//��󳤶�
	BYTE							cbTractorData[MAX_CARD_COUNT*3/2];	//��������
};

//ը����Ϣ
struct tagBoomInfo
{
	BYTE		cbBoomType[MAX_BOOM_NUM];	//ը������
	BYTE		cbBoomNum;					//ը������
};

//��ҿ��Գ��Ŀ�����Ϣ
struct tagOutCardNotify
{
	BYTE		cbOutCardNum;						//��ұ�����Ƶ���Ŀ
	BYTE		cbMustOutCard[MAX_CARD_COUNT];		//��ұ�����Ŀ���
	BYTE		cbOptionalCard[MAX_CARD_COUNT];		//��ҿ�ѡ���Ŀ���
};

//�����ṹ
struct tagAnalyseResult
{
	BYTE 							cbFourCount;						//������Ŀ
	BYTE 							cbThreeCount;						//������Ŀ
	BYTE 							cbDoubleCount;						//������Ŀ
	BYTE							cbSignedCount;						//������Ŀ
	BYTE							cbFourCardData[60];					//�����˿�
	BYTE							cbThreeCardData[200];				//�����˿�
	BYTE							cbDoubleCardData[200];				//�����˿�
	BYTE							cbSignedCardData[200];				//�����˿�
	bool							cbLaiZi;							//��������
};

//�������ṹ
struct tagCardRecorder
{
	BYTE							TwoCount;					//��2������
	BYTE							ThreeCount;					//��3������
	BYTE							FourCount;					//��4������
	BYTE							FiveCount;					//��5������
	BYTE							SixCount;					//��6������
	BYTE							SevenCount;					//��7������
	BYTE							EightCount;					//��8������
	BYTE							NineCount;					//��9������
	BYTE							TenCount;					//��10������
	BYTE							JCount;						//��J������
	BYTE							QCount;						//��Q������
	BYTE							KCount;						//��K������
	BYTE							ACount;						//��A������
	BYTE							SmallJokerCount;			//��С��������
	BYTE							BigJokerCount;				//�ƴ���������
};

//�������
struct tagTableCfg
{
	//tagCommonRoomRule com_rule; //ͨ�÷������
	//MSG_SUB_ROOM_RULE sub_rule; //����Ϸ���з������
};



//==============================���������==================================================

//�������ö��
enum CardGroupType
{
	cgERROR = -1,						            //��������
	cgZERO = 0,						                //��������
	cgSINGLE = 1,									//��������
	cgDOUBLE = 2,									//��������
	cgTHREE = 3,									//��������
	cgSINGLE_LINE = 4,								//��������
	cgDOUBLE_LINE = 5,								//��������
	cgTHREE_LINE = 6,								//��������
	cgTHREE_TAKE_ONE = 7,							//����һ��
	cgTHREE_TAKE_TWO = 8,							//����һ��
	cgTHREE_TAKE_ONE_LINE = 9,						//����һ����
	cgTHREE_TAKE_TWO_LINE = 10,						//����һ����
	cgFOUR_TAKE_ONE = 11,							//�Ĵ�����
	cgFOUR_TAKE_TWO = 12,							//�Ĵ�����
	cgBOMB_CARD = 13,							    //ը������
	cgKING_CARD = 14								//��ը����
};

//�������
#define HandCardMaxLen 20
//��ֵ��Сֵ
#define MinCardsValue -25
//��ֵ���ֵ
#define MaxCardsValue 106

#endif