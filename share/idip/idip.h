#ifndef _H_IDIP_H__
#define _H_IDIP_H__

#include <iomanip>

enum EIdipServer
{
	IDIP_SERVER_NONE = 0,//MSֱ�Ӵ�����
	IDIP_SERVER_MS = 1,//MS��ȡ���ݴ���
	IDIP_SERVER_GS = 2,//game server
	IDIP_SERVER_LOGIN = 3,// login server
	IDIP_SERVER_OTHER = 4,//other
};
enum EIdipNoticeType
{
	NOTICE_TYPE_BEFORE_GAME = 1,	//��Ϸǰ����
	NOTICE_TYPE_AUTHORIZE = 2,		//��Ȩ�󹫸�
	NOTICE_TYPE_ENTER_GAME  = 3,    //������Ϸ�󹫸�(����)
	NOTICE_TYPE_ENTER_GAME_PIC = 4, //������Ϸ�󹫸�(ͼƬ)
	NOTICE_TYPE_ENTER_FACE = 5,		//����ͼ
	NOTICE_TYPE_ENTER_GAME_TITLEPIC = 6, //������Ϸ�󹫸棨ͼƬ���⣩
};

#define MAX_NOTICECONTENT_LEN 2500 //����ĳ���
#define MAX_BANINFOLIST_NUM 50
#define MAX_GUILDMEMBERLIST_NUM 50 //�����Ա�б�����鳤��
#define FOREVER_TIME 10 * 365 * 86400 //����Ĭ��10��
#define MAX_NOTICELIST_NUM 3
#define MAX_PARTITIONLIST_NUM 100
#define MAX_BAGLIST_NUM 200 //���������б�����鳤��
#define MAX_DRAGONJADENUM 4 //�������
#define MAX_BADGEATTRNUM 3 //����ϴ�����Ը���;
#define MAX_REFINEATTRNUM 1 //��������;
#define MAX_SMELTATTRNUM 2 //ϴ������;
#define MAX_MAILLIST_NUM 15 //�ʼ���Ϣ�б�����鳤��
#define MAX_FAIRYLIST_NUM 30 //�����б�����鳤��
#define MAX_EQUIPITEMLIST_NUM 50 //װ���б�����鳤��


#define GODDNESS_TEARS_ID 88 //Ů�������ID
enum EIdipScoreType
{
	SCORE_TYPE_DRAGON_COIN = 1,
	SCORE_TYPE_ARENA_COIN = 2,
	SCORE_TYPE_GUILD_CONTRIBUTE = 3,
	SCORE_TYPE_GODDNESS_TEARS = 4,
	SCORE_TYPE_PKPOINT = 5,
	SCORE_TYPE_ARENA_POINT = 6,
	SCORE_TYPE_ALL = 99,
};

enum EIdipGameNoticeType
{
	GAME_NOTICE_TYPE_SYSTEM = 0, //ϵͳ;
	GAME_NOTICE_TYPE_WORLD = 1,  //����;
	GAME_NOTICE_TYPE_LAMP = 2,	 //�����;
	GAME_NOTICE_TYPE_ALL = 99,   //ȫ��
};

enum EIdipBanRankType
{
	BanRankTypeWorldBoss = 1,			//����boss
	BanRankTypeRoleGuildBoss = 2,		//����boss
	BanRankTypeALL = 99,
};

enum EIdipBanPlayType
{
	BanPlayTypeDailyPlay = 1,			//�ճ��淨
	BanPlayTypeMutilActivity = 2,		//�����淨
	BanPlayTypeALL = 99,				//ȫѡ
};

enum EIdipBanOfflineRankType
{
	BanOfflineRankType_Pk = 1,			//����
	BanOfflineRankType_Arena = 2,		//������
	BanOfflineRankType_Tower = 3,		//�ڰ����
	BanOfflineRankType_Flower = 4,		//����
	BanOfflineRankType_Guild = 5,		//����
	BanOfflineRankType_GuildBoss = 6,	//����boss
	BanOfflineRankType_ALL = 99,		//all
};

enum EIdipDoDelItemType
{
	DoDelItemType_Fashion = 1,	//ʱװ
	DoDelItemType_Item = 2,		//װ��
	DoDelItemType_Emblem = 3,	//����
	DoDelItemType_Jade = 4,		//װ���ϵ�����
};

enum EIdipCmd
{
	IDIP_DO_UPDATE_GOLD_REQ = 4137,							//�޸Ľ�ң���Ϸ�ң�
	IDIP_DO_UPDATE_GOLD_RSP = 4138,							//

	IDIP_DO_UPDATE_EXP_REQ  = 4139,							//�޸ľ���ֵ
	IDIP_DO_UPDATE_EXP_RSP	= 4140,

	IDIP_DO_UPDATE_PHYSICAL_REQ = 4141,						//�޸�����ֵ
	IDIP_DO_UPDATE_PHYSICAL_RSP = 4142,

	IDIP_DO_SEND_ITEM_REQ = 4143,							//���͵���
	IDIP_DO_SEND_ITEM_RSP = 4144,

	IDIP_DO_DEL_ITEM_REQ = 4145,							//�۳�����
	IDIP_DO_DEL_ITEM_RSP = 4146,

	IDIP_DO_ACTIVE_USR_REQ = 4147,							//�����ʺ�
	IDIP_DO_ACTIVE_USR_RSP = 4148,

	IDIP_DO_BAN_USR_REQ = 4149,								//���
	IDIP_DO_BAN_USR_RSP = 4150,

	IDIP_DO_UNBAN_USR_REQ = 4151,						    //������
	IDIP_DO_UNBAN_USR_RSP = 4152,

	IDIP_QUERY_USER_INFO_REQ = 4153,						//��ǰ������Ϣ��ѯ
	IDIP_QUERY_USER_INFO_RSP = 4154,

	IDIP_QUERY_REVERSE_USER_INFO_REQ = 4155,				//��ǰ������Ϣ���ѯ
	IDIP_QUERY_REVERSE_USER_INFO_RSP = 4156,

	IDIP_QUERY_ROLE_INFO_REQ = 4157,						//��ǰ��ɫ��Ϣ��ѯ
	IDIP_QUERY_ROLE_INFO_RSP = 4158,

	IDIP_QUERY_PARTITION_INFO_REQ = 4159,					//С����ѯ
	IDIP_QUERY_PARTITION_INFO_RSP = 4160,

	IDIP_QUERY_BAN_INFO_REQ = 4161,							//��ѯȫ�����״̬����
	IDIP_QUERY_BAN_INFO_RSP = 4162,

	IDIP_DO_UPDATE_DIAMOND_REQ = 4163,						//�޸���ʯ�����ң�
	IDIP_DO_UPDATE_DIAMOND_RSP = 4164,						//
	 
	IDIP_DO_UPDATE_DRAGON_GOLD_REQ = 4165,					//�޸����� 
	IDIP_DO_UPDATE_DRAGON_GOLD_RSP = 4166,

	IDIP_DO_UPDATE_HONOUR_REQ = 4167,						//�޸�����ֵ
	IDIP_DO_UPDATE_HONOUR_RSP = 4168,

	IDIP_DO_MASK_CHAT_REQ = 4169,							//��������
	IDIP_DO_MASK_CHAT_RSP = 4170,

	IDIP_QUERY_BAG_REQ = 4171,								//��ǰ������ѯ
	IDIP_QUERY_BAG_RSP = 4172,

	IDIP_QUERY_BADGE_REQ = 4173,							//��ǰ���²�ѯ
	IDIP_QUERY_BADGE_RSP = 4174,

	IDIP_QUERY_RUNNING_NOTICE_REQ = 4175,					//��ѯ����
	IDIP_QUERY_RUNNING_NOTICE_RSP = 4176,					

	IDIP_QUERY_EQUIPMENT_REQ = 4177,						//��ǰװ����ѯ
	IDIP_QUERY_EQUIPMENT_RSP = 4178,

	IDIP_DO_SEND_NOTICE_REQ = 4179,							//���͹���
	IDIP_DO_SEND_NOTICE_RSP = 4180,
	
	IDIP_DO_SEND_ROLLING_MSG_REQ = 4181,					//��������ƹ���
	IDIP_DO_SEND_ROLLING_MSG_RSP = 4182,

	IDIP_QUERY_RUNNING_ROLLING_MSG_REQ = 4183,				//��ѯ����ƹ���
	IDIP_QUERY_RUNNING_ROLLING_MSG_RSP = 4184,

	IDIP_DO_STOP_ROLLING_MSG_REQ = 4185,					//ɾ������ƹ���
	IDIP_DO_STOP_ROLLING_MSG_RSP = 4186,

	IDIP_QUERY_PARTY_INFO_REQ = 4187,						//��ѯ������Ϣ
	IDIP_QUERY_PARTY_INFO_RSP = 4188,

	IDIP_DO_GM_UPDATE_DIAMOND_REQ = 4189,					//�޸���ʯ�����ң����������
	IDIP_DO_GM_UPDATE_DIAMOND_RSP = 4190,

	IDIP_DO_GM_UPDATE_DRAGON_GOLD_REQ =	4191,				//�޸����ң����ң����������
	IDIP_DO_GM_UPDATE_DRAGON_GOLD_RSQ = 4192,

	IDIP_DO_GM_SEND_ITEM_REQ = 4193,						//���͵���
	IDIP_DO_GM_SEND_ITEM_RSP = 4194,

	IDIP_DO_GM_UPDATE_PHYSICAL_REQ = 4195,					//�޸�����ֵ������ˣ�����
	IDIP_DO_GM_UPDATE_PHYSICAL_RSP = 4196,

	IDIP_AQ_QUERY_OPENID_INFO_REQ = 4197,					//��ѯopenid������Ϣ��AQ������
	IDIP_AQ_QUERY_OPENID_INFO_RSP = 4198,

	IDIP_AQ_QUERY_USER_INFO_REQ = 4199,						//��ѯ�˺��½�ɫ��Ϣ��AQ������
	IDIP_AQ_QUERY_USER_INFO_RSP = 4200,

	IDIP_AQ_DO_SEND_MSG_REQ = 4201,							//����Ϣ��AQ������
	IDIP_AQ_DO_SEND_MSG_RSP = 4202,

	IDIP_AQ_DO_UPDATE_GOLD_COIN_REQ = 4203,					//�޸���Ϸ��������AQ������
	IDIP_AQ_DO_UPDATE_GOLD_COIN_RSP = 4204,

	IDIP_AQ_DO_UPDATE_DIAMOND_REQ = 4205,					//�޸���ʯ������AQ������
	IDIP_AQ_DO_UPDATE_DIAMOND_RSP = 4206,

	IDIP_AQ_DO_CLEAR_SCORE_REQ = 4207,						//������Ϸ������AQ������
	IDIP_AQ_DO_CLEAR_SCORE_RSP = 4208,

	IDIP_AQ_DO_SET_SCORE_REQ = 4209,						//������Ϸ������AQ������
	IDIP_AQ_DO_SET_SCORE_RSP = 4210,

	IDIP_AQ_QUERY_BOSS_BATTLE_INFO_REQ = 4211,				//��ѯBossս��ǰ��Ϣ��AQ������
	IDIP_AQ_QUERY_BOSS_BATTLE_INFO_RSP = 4212,

	IDIP_AQ_DO_BAN_RANKING_REQ = 4213,						//��ֹʵʱ���а�
	IDIP_AQ_DO_BAN_RANKING_RSP = 4214,

	IDIP_AQ_QUERY_PARTY_NOTICE_REQ = 4215,				    //��ѯ���ṫ�棨AQ��Ӧ��
	IDIP_AQ_QUERY_PARTY_NOTICE_RSP = 4216,

	IDIP_AQ_DO_SET_NOTICE_REQ = 4217,						//�����û�ǩ���͹��ṫ�棨AQ������
	IDIP_AQ_DO_SET_NOTICE_RSP = 4218,

	IDIP_AQ_DO_INITIAL_ACCOUNT_REQ = 4219,					//��ʼ���ʺ�
	IDIP_AQ_DO_INITIAL_ACCOUNT_RSP = 4220,

	IDIP_AQ_DO_ZEROPROFIT_REQ = 4221,						//������
	IDIP_AQ_DO_ZEROPROFIT_RSP = 4222,

	IDIP_AQ_DO_BAN_PLAY_REQ = 4223,							//��ָֹ���淨��AQ������
	IDIP_AQ_DO_BAN_PLAY_RSP = 4224,

	IDIP_AQ_DO_BAN_ALL_PLAY_REQ = 4225,						//��ֹ�����淨�ӿڣ�AQ������
	IDIP_AQ_DO_BAN_ALL_PLAY_RSP = 4226,

	IDIP_AQ_DO_BAN_USR_REQ = 4227,							//��ţ�AQ������
	IDIP_AQ_DO_BAN_USR_RSP = 4288,


	IDIP_AQ_DO_MASK_CHAT_REQ = 4229,						//���ԣ�AQ������
	IDIP_AQ_DO_MASK_CHAT_RSP = 4230,
	
	IDIP_AQ_DO_CLEAR_SPEAK_REQ = 4231,						//���������AQ������
	IDIP_AQ_DO_CLEAR_SPEAK_RSP = 4232,


	IDIP_AQ_DO_SEND_NOTICE_REQ = 4233,						//���͹��棨AQ������
	IDIP_AQ_DO_SEND_NOTICE_RSP = 4234,

	IDIP_AQ_DO_RELIEVE_PUNISH_REQ = 4235,					//���������AQ������
	IDIP_AQ_DO_RELIEVE_PUNISH_RSP = 4236,

	IDIP_AQ_DO_BAN_RANKING_OFFLINE_REQ = 4237,				//��ֹ�������а�ӿڣ����߰񣩣�AQ������
	IDIP_AQ_DO_BAN_RANKING_OFFLINE_RSP = 4238,		

	IDIP_DO_GM_UPDATE_GOLD_REQ = 4239,						//�޸Ľ�ң����ң����������
	IDIP_DO_GM_UPDATE_GOLD_RSP = 4240,

	IDIP_QUERY_INDIVIDUAL_BAN_INFO_REQ = 4241,				//��ѯ���˷����Ϣ����
	IDIP_QUERY_INDIVIDUAL_BAN_INFO_RSP = 4242,

	IDIP_QUERY_GUILD_INFO_REQ = 4243,						//��ѯ������Ϣ����
	IDIP_QUERY_GUILD_INFO_RSP = 4244,

	IDIP_QUERY_GUILD_MEMBER_INFO_REQ = 4245,				//��ѯ�����Ա��Ϣ����
	IDIP_QUERY_GUILD_MEMBER_INFO_RSP = 4246,

	IDIP_QUERY_FAIRY_INFO_REQ = 4247,						//��ǰ������Ϣ��ѯ����
	IDIP_QUERY_FAIRY_INFO_RSP = 4248,

	IDIP_QUERY_HORSE_INFO_REQ = 4249,						//��ǰ������Ϣ��ѯ����
	IDIP_QUERY_HORSE_INFO_RSP = 4250,

	IDIP_DO_UNMASK_CHAT_REQ = 4251,							//�����������
	IDIP_DO_UNMASK_CHAT_RSP = 4252,

	IDIP_DO_SEND_MAIL_REQ = 4253,							//ȫ���ʼ�����
	IDIP_DO_SEND_MAIL_RSP = 4254,

	IDIP_DO_CONTROL_FUNCTION_REQ = 4259,					//���ܿ�������
	IDIP_DO_CONTROL_FUNCTION_RSP = 4260,

	IDIP_DO_NAME_ACT_REQ = 4261,							//�ƺż�������
	IDIP_DO_NAME_ACT_RSP = 4262,

	IDIP_QUERY_FASHION_REQ = 4263,							//��ǰʱװ��ѯ����
	IDIP_QUERY_FASHION_RSP = 4264,

	IDIP_QUERY_GUILD_INFO_BACK_REQ = 4265,					//������Ϣ���ѯ����
	IDIP_QUERY_GUILD_INFO_BACK_RSP = 4266,

	IDIP_DO_DEL_ITEM_OUTBAG_REQ = 4267,						//ɾ����������ߣ�ʱװ�����£�װ��������
	IDIP_DO_DEL_ITEM_OUTBAG_RSP = 4268,

	IDIP_DO_SEND_PERSON_MAIL_REQ = 4269,					//�����ʼ�����
	IDIP_DO_SEND_PERSON_MAIL_RSP = 4270,

	IDIP_QUERY_USER_INFO_WEEK_REQ = 4277,					//���ܸ�����Ϣ��ѯ����
	IDIP_QUERY_USER_INFO_WEEK_RSP = 4278,

	IDIP_DO_SET_NO_PLAY_REQ = 4279,							//�ɳ��ػ�����������������
	IDIP_DO_SET_NO_PLAY_RSP = 4280,

	IDIP_QUERY_GUILD_TASK_REQ = 4281,						//��ǰ���������ѯ����;
	IDIP_QUERY_GUILD_TASK_RSP = 4282,

	IDIP_DO_XINYUE_RED_REQ = 4283,							//���ú�㼤������
	IDIP_DO_XINYUE_RED_RSP = 4284,

	IDIP_DO_UPDATE_NAME_REQ = 4285,							//��������޸�����
	IDIP_DO_UPDATE_NAME_RSP = 4286,

	IDIP_DO_UPDATE_GUILD_NAME_REQ = 4287,					//���������޸�����
	IDIP_DO_UPDATE_GUILD_NAME_RSP = 4288,

	IDIP_DO_XINYUE_R_RECHARGE_REQ = 4289,					//�޸���ʯ�����ң�����R���䣩����
	IDIP_DO_XINYUE_R_RECHARGE_RSP = 4290,

	IDIP_DO_CHANGE_REQ = 4291,								//���߶һ�����
	IDIP_DO_CHANGE_RSP = 4292,

	IDIP_DO_UPDATE_PRESTIGE_REQ = 4293,						//�޸Ĺ�����������
	IDIP_DO_UPDATE_PRESTIGE_RSP = 4294,

	IDIP_DO_UPDATE_GUILD_EXP_REQ = 4295,					//�޸Ĺ��ᾭ������
	IDIP_DO_UPDATE_GUILD_EXP_RSP = 4296,

	IDIP_QUERY_MAIL_STATUS_REQ = 4297,						//��ǰ����ʼ�״̬��ѯ����
	IDIP_QUERY_MAIL_STATUS_RSP = 4298,

	IDIP_DO_UPDATE_BACK_ATTR_REQ = 4299,					//�޸���һع����������
	IDIP_DO_UPDATE_BACK_ATTR_RSP = 4300,

	IDIP_QUERY_BACK_ATTR_REQ = 4301,						//��ѯ��һع����������
	IDIP_QUERY_BACK_ATTR_RSP = 4302,

	IDIP_QUERY_ITEM_ID_TO_NAME_REQ = 4303,					//����ID���������������
	IDIP_QUERY_ITEM_ID_TO_NAME_RSP = 4304,

	IDIP_QUERY_FIGHT_PUNISH_REQ = 4305,						//��ѯӢ��ս�������������
	IDIP_QUERY_FIGHT_PUNISH_RSP = 4306,

	IDIP_DO_CLEAR_FIGHT_PUNISH_REQ = 4307,					//���Ӣ��ս����������
	IDIP_DO_CLEAR_FIGHT_PUNISH_RSP = 4308,

	IDIP_DO_XIAOYUE_RED_REQ = 4309,							//С�ú�㼤������
	IDIP_DO_XIAOYUE_RED_RSP = 4310,

	IDIP_DO_ACTIVE_ACHIEVEMENT_REQ = 4311,					//�ɾͼ�������
	IDIP_DO_ACTIVE_ACHIEVEMENT_RSP = 4312,

	IDIP_DO_ADD_DEL_WHITE_LIST_REQ = 4313,					//�����ɾ������������
	IDIP_DO_ADD_DEL_WHITE_LIST_RSP = 4314,

	IDIP_QUERY_1V1_RANK_REQ = 4317,							//��ѯ����1v1��������
	IDIP_QUERY_1V1_RANK_RSP = 4318,

	IDIP_QUERY_WHITE_LIST_REQ = 4319,						//��ѯ�������������
	IDIP_QUERY_WHITE_LIST_RSP = 4320,

	IDIP_DO_COIN_CONSUME_TO_GAME_REQ = 4321,				//���������ϱ���Ϸ������
	IDIP_DO_COIN_CONSUME_TO_GAME_RSP = 4322,

	IDIP_QUERY_ROLE_CLOTHES_REQ = 4323,						//��ɫʱװ�ϳ������ѯ����
	IDIP_QUERY_ROLE_CLOTHES_RSP = 4324,
};


enum EIdipErrCode
{
	IDIP_ERR_SUCCESS							= 0,		//�ɹ�
	IDIP_ERR_PLAYER_NOT_EXIST					= 1,		//�˺Ż��ɫ������
	IDIP_ERR_ROLE_OFFLINE						= 2,		//��ɫ������
	IDIP_ERR_INVALID_PARA						= 3,		//��Ч���������
	IDIP_ERR_ALREADY_ACTIVE						= 4,		//�ѱ������
	IDIP_ERR_SEND_MAIL							= 5,		//�����ʼ�ʧ��
	IDIP_ERR_INVALID_CMDID						= 6,		//û�и�Э��ID

	IDIP_ERR_UNKNOWN							= 10,		//��������
	IDIP_ERR_INVALID_REQ_NUM					= 11,		//���������Ƿ�
	IDIP_ERR_INVALID_ITEM_ID					= 20,		//����ID�Ƿ�
	IDIP_ERR_INVALID_OPEN_ID					= 21,		//OpenID�Ƿ�
	IDIP_ERR_INVALID_TIME						= 22,		//ʱ���ʽ�Ƿ�
	IDIP_ERR_INVALID_LAMPID						= 23,		//��������Ч������ƹ���
	IDIP_ERR_NONE_LAMPNOTICE					= 24,		//û�н��е�����ƹ���
	IDIP_ERR_INVALID_GUILDID					= 25,		//��Ч�Ĺ���ID
	IDIP_ERR_INVALID_SERVERID					= 26,		//��Ч�ķ�����ID
	IDIP_ERR_MAX_UPDATE_NUM						= 27,		//��������޸�����;
	IDIP_ERR_NOTJOIN_GUILD						= 28,		//û�м��빫��;
	IDIP_ERR_TIMEOUT							= 29,		//��ʱ
	IDIP_ERR_NUM_NOT_ENOUGH						= 30,		//��������
	IDIP_ERR_INVALID_ORDERID					= 31,		//��Ч����ˮ��
	IDIP_ERR_KICKOUT							= 32,		//������ʧ��
	IDIP_ERR_NOLOGIN							= 33,		//idipserver��login��ͨ;
	IDIP_ERR_NOMS								= 34,		//idipserver����Ϸ��������ͨ;
	IDIP_ERR_SAME_LAMPID						= 35,		//�Ѵ��ڸ�id�������
};

inline std::string UrlEncode(const std::string& src)
{
	std::stringstream ss;
	for(std::string::size_type i = 0; i < src.size(); ++i)
	{
		INT32 val = (INT32)src[i];
		val &= 0x000000FF;
		ss << "%" << std::setiosflags(std::ios::uppercase) << std::hex << std::setw(2) << std::setfill('0') << val;
	}
	return ss.str();
}
inline BYTE toHex(const BYTE &x)
{
	return x > 9 ? x -10 + 'A': x + '0';
}

inline BYTE fromHex(const BYTE &x)
{
	return isdigit(x) ? x-'0' : x-'A'+10;
}
inline std::string UrlDecode(const std::string &sIn)
{
	std::string sOut;
	for( size_t ix = 0; ix < sIn.size(); ix++ )
	{
		BYTE ch = 0;
		if(sIn[ix]=='%' && (sIn.size() - 1 - ix) >= 2)
		{
			ch = (fromHex(sIn[ix+1])<<4);
			ch |= fromHex(sIn[ix+2]);
			ix += 2;
		}
		else if(sIn[ix] == '+')
		{
			ch = ' ';
		}
		else
		{
			ch = sIn[ix];
		}
		sOut += (char)ch;
	}
	return sOut;
}

#endif