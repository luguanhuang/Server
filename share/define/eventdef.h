#ifndef __EVENTDEF_H__
#define __EVENTDEF_H__




namespace GlobalEvent
{
	enum EventOp
	{
		EVENT_OP_ADD	= 0,
		EVENT_OP_DEL	= 1,
		EVENT_OP_MODIFY = 2,
	};

	struct stEvent
	{
		stEvent()
		{
			nID		= 0;
			nTime	= 0;
			nParam1 = 0;
			nParam2 = 0;
			nParam3 = 0;
			lParam1 = 0;
			lParam2 = 0;
			op		= 0;
			szName  = "";
		}
		UINT32 nID;
		UINT32 nTime;
		UINT32 nParam1;
		UINT32 nParam2;
		UINT32 nParam3;
		UINT64 lParam1;
		UINT64 lParam2;
		UINT32 op;
		std::string szName;
		bool operator<(const stEvent event)const
		{
			if (nID < event.nID)
				return true;
			else if(nID == event.nID&& nTime < event.nTime)
				return true;
			return false;
		}
	};

	typedef std::function<bool(void)> MasterEventCallBack;
	typedef std::function<bool(stEvent *)> GameEventCallBack;

	struct stMasterEvent:public stEvent
	{
		MasterEventCallBack callback;
		UINT32 nCD;
	};

	struct stGameEvent:public stEvent
	{
		GameEventCallBack callback;
	};

};

#define ACH_EVENT_OFFSET		1000			//1000�Ժ��ǳɾ�

#define BONUS_EVENT_OFFSET		2000			//2000�Ժ��Ǻ��

#define MENTOR_EVENT_OFFSET		4000			//4000�Ժ���ʦͽ

#define MIX_EVENT_OFFSET		5000			//5000�Ժ������Ӱ�

#define TASK_EVENT_OFFSET		6000			//6000�Ժ�������

//����������
enum
{
	GUILDBONUS_COM_TYPE_PK_WEEK_RANK			= 1, //����
	GUILDBONUS_COM_TYPE_KILLDRAGON				= 2, //�ص׾���
	GUILDBONUS_COM_TYPE_CHARGE					= 3, //��ֵ
	GUILDBONUS_COM_TYPE_VIPLEVEL				= 4, //vip level
	GUILDBONUS_COM_TYPE_DRAWLOTTERY_TEN			= 5, //ʮ������Ʒ
	GUILDBONUS_COM_TYPE_TITLE					= 6, //ͷ��
	GUILDBONUS_COM_TYPE_STRENGTHEN				= 7, //ǿ��
	GUILDBONUS_COM_TYPE_TOWER					= 8, //����
	GUILDBONUS_COM_TYPE_PET					    = 9, //����
	GUILDBONUS_COM_TYPE_PRIVILEGE			    = 10,//��Ȩ
	GUILDBONUS_COM_TYPE_FUND					= 11,//����
	GUILDBONUS_COM_TYPE_ALLDRAGONJADE_LEVEL		= 12,//ȫ������ȼ�
	GUILDBONUS_COM_TYPE_GUILDGMF_RANK		    = 13,//������̨ս����
	GUILDBONUS_COM_TYPE_SKYARENA_FLOOR		    = 14,//��վ�����
	GUILDBONUS_COM_TYPE_DRAGON_NEST		        = 15,//��Ѩ
	GUILDBONUS_COM_TYPE_BOSSRUSH		        = 16,//bossrush
	GUILDBONUS_COM_TYPE_BUYIBSHOP				= 17, //�ϴι���
	GUILDBONUS_COM_TYPE_MAYHEMRANK		        = 18,//������Ҷ�����
	GUILDBONUS_COM_TYPE_USETHREESUIT		    = 19,//S������
	GUILDBONUS_COM_TYPE_USEITEM					= 20,//ʹ�õ���
	GUILDBONUS_COM_TYPE_MAX, 

};

///>�ɾ��������
enum
{
	/*
	1=�������
	2=�����װ����
	3=���������װ����
	4=����һ��װ��ǿ���ȼ�
	5=����һ����װǿ���ȼ�
	6=תְ
	7=��ҵȼ�
	8=ͨ��ĳһ�½�
	9=��ʱ���ĳ�ؿ�
	10=�Ǽ����ĳ�ؿ�
	11=����ͨ��ĳ�ؿ�
	12=1��ͨ��ĳ�ؿ�
	*/

	ACHIEVE_COM_TYPE_JOIN_GUILD				= 1, 
	ACHIEVE_COM_TYPE_SUIT_COUNT				= 2, 	
	ACHIEVE_COM_TYPE_JEWELRY_SUIT_COUNT		= 3, 	
	ACHIEVE_COM_TYPE_EQUIP_LEVEL			= 4, 	
	ACHIEVE_COM_TYPE_SUIT_LEVEL				= 5, 	
	ACHIEVE_COM_TYPE_CHANGE_JOB				= 6, 	
	ACHIEVE_COM_TYPE_LEVEL					= 7, 	
	ACHIEVE_COM_TYPE_FINISH_CHAPTER			= 8, 	
	ACHIEVE_COM_TYPE_LIMIT_TIME_FINISH_RAID	= 9, 	
	ACHIEVE_COM_TYPE_STAR_FINISH_RAID		= 10, 	
	ACHIEVE_COM_TYPE_NO_HURT_FINISH_RAID	= 11, 	
	ACHIEVE_COM_TYPE_ONE_STAR_FINSIH_RAID   = 12, 


	/*
	13=�ͻ�����
	14=ʱװ����
	15=ʱװƷ��
	16=����ͨ��
	17=����ͨ��
	18=����BOSS
	19=3S������ͨ��
	20=����������ͨ��
	21=��������ʤ����
	22=����������
	23=��̨������
	25=��������
	26=�ۻ���½����
	27=���Ѵﵽһ�����
	28=�׳�
	29=��ս
	30=����
	*/
	ACHIEVE_COM_TYPE_GIVE_FLOWER				= 13, 	
	ACHIEVE_COM_TYPE_FASHION_COUNT				= 14, 	
	ACHIEVE_COM_TYPE_FASHION_QUALITY			= 15, 	
	ACHIEVE_COM_TYPE_NO_DIE_FINISH_RAID			= 16, 	
	ACHIEVE_COM_TYPE_ALONE_FINISH_RAID			= 17, 	
	ACHIEVE_COM_TYPE_KILL_BOSS					= 18, 	
	ACHIEVE_COM_TYPE_3S_FINISH_RAID				= 19, 	
	ACHIEVE_COM_TYPE_FOUR_NO_DIE_FINISH_RAID	= 20, 	
	ACHIEVE_COM_TYPE_PK_WIN_TIMES				= 21, 	
	ACHIEVE_COM_TYPE_PK_SCORE					= 22, 	
	ACHIEVE_COM_TYPE_ARENA_RANK					= 23, 	
	ACHIEVE_COM_TYPE_TOWER_FLOOR				= 24, 	
	ACHIEVE_COM_TYPE_TOTAL_LOGIN				= 25, 	
	ACHIEVE_COM_TYPE_RECHARGE_AMOUNT			= 26, 	
	ACHIEVE_COM_TYPE_FIRST_RECHARGE				= 27, 	
	ACHIEVE_COM_TYPE_ASSIST						= 28,
	ACHIEVE_COM_FIRST_PASS						= 29,
	ACHIEVE_COM_DRAGON_SCENE					= 30,
	ACHIEVE_COM_JEWELRY_SUIT_LEVEL				= 31,
	ACHIEVE_COM_GARDEN							= 32,
	ACHIEVE_COM_MENTOR							= 33,
	ACHIEVE_COM_CHAPTERID						= 34,
	ACHIEVE_COM_MARRY							= 35,
	ACHIEVE_COM_IDIP_ADD							= 500,
	ACHIEVE_COM_IDIP_DEL							= 501,
	ACHIEVE_COM_TYPE_MAX, 
};

//�ƺųɾ�






	//�ƺŴ���;
enum
{
	/*
	1=��ͨ�ƺ�;
	2=�����ƺ�;
	3=��Ѩ�ƺ�;
	4=ս���ƺ�;
	5=��ƺ�;
	*/
	DESIGNAION_BIG_TYPE_COMMON		= 1,
	DESIGNAION_BIG_TYPE_RAID				= 2, 
	DESIGNAION_BIG_TYPE_NEST				= 3, 
	DESIGNAION_BIG_TYPE_BATTLE			= 4, 
	DESIGNAION_BIG_TYPE_ACT				= 5, 
	DESIGNAION_BIG_TYPE_TIMELIMIT		= 6,
	DESIGNAION_BIG_TYPE_MAX 
};

//�ƺ��������;
enum
{
	/*
	1=�ȼ��ﵽ���ټ�;
	2=����װ��ǿ�������ټ�;
	3=��ö��ټ�װ����װ;
	4=��ö��ټ�������װ;
	5=ͨ�ص�X���������߹ؿ�;
	6=�ؿ���ʱ���;
	7=�ؿ�����ͨ��;
	8=�ؿ�����ͨ��;
	9=ӵ�ж��ٽ��;
	10=���ʱװ;
	11=�ؿ�1��ͨ��;
	*/

	DESIGNATION_COM_TYPE_LEVEL									= 1, 
	DESIGNATION_COM_TYPE_ANY_EQUIP_LEVEL				= 2, 	
	DESIGNATION_COM_TYPE_SUIT_EQUIP_COUNT				= 3, 
	DESIGNATION_COM_TYPE_JEWELRY_COUNT					= 4,	
	DESIGNATION_COM_TYPE_FINISH_CHAPTER					= 5, 
	DESIGNATION_COM_TYPE_LIMIT_TIME_FINISH_RAID		= 6, 
	DESIGNATION_COM_TYPE_3_STAR_FINISH_RAID			= 7, 
	DESIGNATION_COM_TYPE_NO_HURT_FINISH_RAID		= 8, 
	DESIGNATION_COM_TYPE_HAVE_GOLD_COUNT			= 9, 
	DESIGNATION_COM_TYPE_HAVE_FASHION_EQUIP		= 10, 
	DESIGNATION_COM_TYPE_ONE_STAR_FINISH_RAID		= 11, 


	/*
	12=ӵ�ж�����ʯ;
	13=�ͳ������ʻ�;
	14=���빫��;
	15=ĳ����˫��������ͨ��;
	16=ĳ��������ͨ��;
	17=����������ͨ�س�Ѩ;
	18=3S������ͨ�س�Ѩ;
	19=��Ѩͨ��������;
	20=��������ʤ����;
	21=����������;
	22=����������;
	23=����ڰ�������;
	24=���ĳ������-�簢������֮��;
	25=������½����;
	26=���������ٻ��;
	27=�׳���;
	*/
	DESIGNATION_COM_TYPE_DIAMOND_COUNT								= 12, 
	DESIGNATION_COM_TYPE_GIVE_FLOWER								= 13, 
	DESIGNATION_COM_TYPE_JOIN_GUILD									= 14, 
	DESIGNATION_COM_TYPE_TWO_NO_DIE_FINISH_RAID						= 15, 
	DESIGNATION_COM_TYPE_ALONE_FINISH_RAID							= 16, 
	DESIGNATION_COM_TYPE_FOUR_NO_DIE_FINISH_RAID					= 17, 
	DESIGNATION_COM_TYPE_3S_FINISH_RAID								= 18, 
	DESIGNATION_COM_TYPE_MOST_HURT_FINISH_RAID						= 19, 
	DESIGNATION_COM_TYPE_SKY_LADDER_WIN_TIMES						= 20, 
	DESIGNATION_COM_TYPE_SKY_LADDER_SCORE							= 21, 
	DESIGNATION_COM_TYPE_SKY_LADDER_RANK							= 22, 
	DESIGNATION_COM_TYPE_TOWER_FLOOR								= 23, 
	DESIGNATION_COM_TYPE_FINISH_TASK								= 24, 
	DESIGNATION_COM_TYPE_TOTAL_LOGIN								= 25, 
	DESIGNATION_COM_TYPE_RECHARGE_AMOUNT							= 26, 
	DESIGNATION_COM_TYPE_FIRST_RECHARGE								= 27, 
	DESIGNATION_COM_TYPE_NO_DIE_FINISH_RAID							= 28, 

	DESIGNATION_COM_TYPE_FLOWER_RANK_YESTERDAY						= 29,	//�����ʻ���;
	DESIGNATION_COM_TYPE_FLOWER_RANK_HISTORY						= 30,	//��ʷ�ʻ���;
	DESIGNATION_COM_TYPE_ARENA_RANK									= 31,	//����������;
	DESIGNATION_COM_TYPE_FIRST_KILL_GUILD_BOSS						= 32,	//�����״λ�ɱ����BOSS;
	DESIGNATION_COM_TYPE_PPT										= 33,	///>ս���ﵽһ������
	DESIGNATION_COM_TYPE_GUILD_GROUP_CHAMPION						= 34,	///>������̨ս�ھ��������г�Ա A����
	DESIGNATION_COM_TYPE_PK_WEEK_RANK								= 35,	///>����������
	DESIGNATION_COM_TYPE_ASSIST										= 36,	///>��ս�ɾ� 
	DESIGNATION_COM_TYPE_FIRSTPASS									= 37,	///>��ͨ
	DESIGNATION_COM_TYPE_GUILD_ACTIVITY_RANK						= 38,   ///>��������
	DESIGNATION_COM_TYPE_WEEK_ASSIST								= 39,   ///>ÿ����ս
	DESIGNATION_COM_TYPE_GARDEN										= 40,	///>garden
	DESIGNATION_COM_DRAGON_SCENE									= 41,	///>����
	DESIGNATION_COM_NESTWEEK										= 42,	///>ÿ�ܳ�Ѩ
	DESIGNATION_COM_GAY												= 43,	///>	
	DESIGNATION_COM_BEGINMIND										= 44,   ///> ��������
	DESIGNATION_COM_GREATMERIT										= 45,	///> ����Ԫѫ	
	DESIGNATION_COM_ORDER											= 46,	///> ԤԼ�ƺ�
	DESIGNATION_COM_GUILDTERRITORYBATTLE							= 47,	///> �������ս
	DESIGNATION_COM_LEAGUELICHAMPTION								= 48,	///> ս������
	DESIGNATION_COM_TGACHAMPTION									= 49,	///> TGA�����ھ�
	DESIGNATION_COM_MENTOR											= 50,	///> ʦͽ
	DESIGNATION_COM_BADGECOLLECTION									= 51,	///> �����ռ�
	DESIGNATION_COM_MILITARY										= 52,	///> ����
	DESIGNATION_COM_QQEXCLUSIVE										= 53,   ///>  qqƽ̨���Ƴƺ�
	DESIGNATION_COM_LIMITASSIST										= 54,	///> ��ʱ�
	DESIGNATION_COM_TYPE_2V2PK_WEEK_RANK							= 55,	///>2v2����������
	DESIGNATION_COM_TYPE_2V2SKY_LADDER_SCORE						= 56,   ///>2v2����������
	DESIGNATION_COM_TYPE_SKYCITY									= 57,   ///>��վ������ƺ�
	DESIGNATION_COM_TYPE_DRAGONASSIST								= 58,   ///>������ս
	DESIGNATION_COM_TYPE_BIGMELEE									= 60,   ///>���Ҷ��ƺ�
	DESIGNATION_COM_TYPE_BIOHELLL									= 61,   ///>������Ѩ
	DESIGNATION_COM_TYPE_MARRY										= 63,   ///>���
	DESIGNATION_COM_TYPE_DNE										= 64,   ///>dne���������ƺ�
	DESIGNATION_COM_TYPE_TOWERRANK									= 65,   ///>�ڰ������һˢ��

	DESIGNATION_COM_TYPE_BATTLEFIELD_RANK							    = 66,   ///>ս��ͳ��
	DESIGNATION_COM_TYPE_BATTLEFIELD_KILL							    = 67,   ///>ս��ͳ��
	DESIGNATION_COM_TYPE_COMPETEDRAGONRANK							    = 68,   ///>������������ˢ��
	DESIGNATION_COM_TYPE_GOALAWARDS										= 69,   ///>Ŀ��ɾ�
	DESIGNATION_COM_TYPE_DRAGON											= 70,   ///>��������
	DESIGNATION_COM_TYPE_HARDCOREDRAGON									= 71,   ///> Ӳ������
	DESIGNATION_COM_TYPE_DRAGONGUILD									= 72,	///> С�ֶ�
	DESIGNATION_COM_TYPE_SURVIVE										= 73,   ///> ��������

	DESIGNATION_COM_TYPE_IDIP_ADD										= 500,   ///>Ǳ���������Ͳ�������ֻ�ܷ������á�
	DESIGNATION_COM_TYPE_IDIP_DEL										= 501,   ///>Ǳ���������Ͳ�������ֻ�ܷ������á�
	DESIGNATION_COM_TYPE_MAX, 
};


enum EVENT_MIX 
{
	EVENT_MIDAS_PAY_DEGREE = 1,												///>�״�ʦ���Ѷ��
	EVENT_MIDAS_PAY_COUNT  = 2,
};

enum TASK_EVENT_ID
{
	TASK_EVENT_SET = 1,
	TASK_EVENT_ADD = 2,
	TASK_EVENT_DEL = 3,
	TASK_EVENT_ADD_TASKID_STEP = 4,		// ֱ�Ӷ�ĳ������ӽ���
	TASK_EVENT_HELP_RECORD = 5,			// ��¼������Ϣ
	TASK_EVENT_DAILY_TASK_REFRESH = 6,	// �ճ�����ˢ��
	TASK_EVENT_CHARGE_BACK = 7,			// ������ֵ����
};

#endif
