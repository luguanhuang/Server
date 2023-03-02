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

#define ACH_EVENT_OFFSET		1000			//1000以后是成就

#define BONUS_EVENT_OFFSET		2000			//2000以后是红包

#define MENTOR_EVENT_OFFSET		4000			//4000以后是师徒

#define MIX_EVENT_OFFSET		5000			//5000以后杂七杂八

#define TASK_EVENT_OFFSET		6000			//6000以后是任务

//红包完成条件
enum
{
	GUILDBONUS_COM_TYPE_PK_WEEK_RANK			= 1, //天梯
	GUILDBONUS_COM_TYPE_KILLDRAGON				= 2, //关底巨龙
	GUILDBONUS_COM_TYPE_CHARGE					= 3, //充值
	GUILDBONUS_COM_TYPE_VIPLEVEL				= 4, //vip level
	GUILDBONUS_COM_TYPE_DRAWLOTTERY_TEN			= 5, //十连抽物品
	GUILDBONUS_COM_TYPE_TITLE					= 6, //头衔
	GUILDBONUS_COM_TYPE_STRENGTHEN				= 7, //强化
	GUILDBONUS_COM_TYPE_TOWER					= 8, //爬塔
	GUILDBONUS_COM_TYPE_PET					    = 9, //坐骑
	GUILDBONUS_COM_TYPE_PRIVILEGE			    = 10,//特权
	GUILDBONUS_COM_TYPE_FUND					= 11,//基金
	GUILDBONUS_COM_TYPE_ALLDRAGONJADE_LEVEL		= 12,//全身龙玉等级
	GUILDBONUS_COM_TYPE_GUILDGMF_RANK		    = 13,//公会擂台战排名
	GUILDBONUS_COM_TYPE_SKYARENA_FLOOR		    = 14,//天空竞技场
	GUILDBONUS_COM_TYPE_DRAGON_NEST		        = 15,//龙穴
	GUILDBONUS_COM_TYPE_BOSSRUSH		        = 16,//bossrush
	GUILDBONUS_COM_TYPE_BUYIBSHOP				= 17, //上次购买
	GUILDBONUS_COM_TYPE_MAYHEMRANK		        = 18,//跨服大乱斗排名
	GUILDBONUS_COM_TYPE_USETHREESUIT		    = 19,//S三件套
	GUILDBONUS_COM_TYPE_USEITEM					= 20,//使用道具
	GUILDBONUS_COM_TYPE_MAX, 

};

///>成就完成条件
enum
{
	/*
	1=公会加入
	2=获得套装件数
	3=获得首饰套装件数
	4=任意一件装备强化等级
	5=任意一件套装强化等级
	6=转职
	7=玩家等级
	8=通关某一章节
	9=限时完成某关卡
	10=星级完成某关卡
	11=无伤通关某关卡
	12=1星通关某关卡
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
	13=送花数量
	14=时装件数
	15=时装品质
	16=不死通关
	17=独立通关
	18=消灭BOSS
	19=3S高评分通关
	20=四人无死亡通关
	21=天梯赛获胜场次
	22=天梯赛积分
	23=擂台赛名次
	25=爬塔层数
	26=累积登陆天数
	27=付费达到一定金额
	28=首冲
	29=助战
	30=龙本
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

//称号成就






	//称号大类;
enum
{
	/*
	1=普通称号;
	2=副本称号;
	3=巢穴称号;
	4=战斗称号;
	5=活动称号;
	*/
	DESIGNAION_BIG_TYPE_COMMON		= 1,
	DESIGNAION_BIG_TYPE_RAID				= 2, 
	DESIGNAION_BIG_TYPE_NEST				= 3, 
	DESIGNAION_BIG_TYPE_BATTLE			= 4, 
	DESIGNAION_BIG_TYPE_ACT				= 5, 
	DESIGNAION_BIG_TYPE_TIMELIMIT		= 6,
	DESIGNAION_BIG_TYPE_MAX 
};

//称号完成条件;
enum
{
	/*
	1=等级达到多少级;
	2=任意装备强化到多少级;
	3=获得多少件装备套装;
	4=获得多少件首饰套装;
	5=通关第X章所有主线关卡;
	6=关卡限时完成;
	7=关卡三星通过;
	8=关卡无伤通关;
	9=拥有多少金币;
	10=获得时装;
	11=关卡1星通过;
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
	12=拥有多少钻石;
	13=送出多少鲜花;
	14=加入公会;
	15=某副本双人无死亡通关;
	16=某副本独自通关;
	17=四人无死亡通关巢穴;
	18=3S高评分通关巢穴;
	19=巢穴通关输出最高;
	20=天梯赛获胜场次;
	21=天梯赛积分;
	22=天梯赛名次;
	23=到达黑暗神殿层数;
	24=完成某个任务-如阿尔杰塔之吻;
	25=连续登陆天数;
	26=付费满多少获得;
	27=首冲获得;
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

	DESIGNATION_COM_TYPE_FLOWER_RANK_YESTERDAY						= 29,	//昨日鲜花榜;
	DESIGNATION_COM_TYPE_FLOWER_RANK_HISTORY						= 30,	//历史鲜花榜;
	DESIGNATION_COM_TYPE_ARENA_RANK									= 31,	//竞技场排名;
	DESIGNATION_COM_TYPE_FIRST_KILL_GUILD_BOSS						= 32,	//参与首次击杀公会BOSS;
	DESIGNATION_COM_TYPE_PPT										= 33,	///>战力达到一定数字
	DESIGNATION_COM_TYPE_GUILD_GROUP_CHAMPION						= 34,	///>公会擂台战冠军公会所有成员 A分组
	DESIGNATION_COM_TYPE_PK_WEEK_RANK								= 35,	///>天梯赛排名
	DESIGNATION_COM_TYPE_ASSIST										= 36,	///>助战成就 
	DESIGNATION_COM_TYPE_FIRSTPASS									= 37,	///>首通
	DESIGNATION_COM_TYPE_GUILD_ACTIVITY_RANK						= 38,   ///>工会活动排名
	DESIGNATION_COM_TYPE_WEEK_ASSIST								= 39,   ///>每周助战
	DESIGNATION_COM_TYPE_GARDEN										= 40,	///>garden
	DESIGNATION_COM_DRAGON_SCENE									= 41,	///>龙本
	DESIGNATION_COM_NESTWEEK										= 42,	///>每周巢穴
	DESIGNATION_COM_GAY												= 43,	///>	
	DESIGNATION_COM_BEGINMIND										= 44,   ///> 不忘初心
	DESIGNATION_COM_GREATMERIT										= 45,	///> 开谷元勋	
	DESIGNATION_COM_ORDER											= 46,	///> 预约称号
	DESIGNATION_COM_GUILDTERRITORYBATTLE							= 47,	///> 领地争夺战
	DESIGNATION_COM_LEAGUELICHAMPTION								= 48,	///> 战队联赛
	DESIGNATION_COM_TGACHAMPTION									= 49,	///> TGA联赛冠军
	DESIGNATION_COM_MENTOR											= 50,	///> 师徒
	DESIGNATION_COM_BADGECOLLECTION									= 51,	///> 徽章收集
	DESIGNATION_COM_MILITARY										= 52,	///> 军衔
	DESIGNATION_COM_QQEXCLUSIVE										= 53,   ///>  qq平台定制称号
	DESIGNATION_COM_LIMITASSIST										= 54,	///> 限时活动
	DESIGNATION_COM_TYPE_2V2PK_WEEK_RANK							= 55,	///>2v2天梯赛排名
	DESIGNATION_COM_TYPE_2V2SKY_LADDER_SCORE						= 56,   ///>2v2天梯赛积分
	DESIGNATION_COM_TYPE_SKYCITY									= 57,   ///>天空竞技场称号
	DESIGNATION_COM_TYPE_DRAGONASSIST								= 58,   ///>龙本助战
	DESIGNATION_COM_TYPE_BIGMELEE									= 60,   ///>大乱斗称号
	DESIGNATION_COM_TYPE_BIOHELLL									= 61,   ///>生化巢穴
	DESIGNATION_COM_TYPE_MARRY										= 63,   ///>结婚
	DESIGNATION_COM_TYPE_DNE										= 64,   ///>dne副本次数称号
	DESIGNATION_COM_TYPE_TOWERRANK									= 65,   ///>黑暗神殿周一刷新

	DESIGNATION_COM_TYPE_BATTLEFIELD_RANK							    = 66,   ///>战场统计
	DESIGNATION_COM_TYPE_BATTLEFIELD_KILL							    = 67,   ///>战场统计
	DESIGNATION_COM_TYPE_COMPETEDRAGONRANK							    = 68,   ///>竞技龙本两周刷新
	DESIGNATION_COM_TYPE_GOALAWARDS										= 69,   ///>目标成就
	DESIGNATION_COM_TYPE_DRAGON											= 70,   ///>竞技龙本
	DESIGNATION_COM_TYPE_HARDCOREDRAGON									= 71,   ///> 硬核龙本
	DESIGNATION_COM_TYPE_DRAGONGUILD									= 72,	///> 小分队
	DESIGNATION_COM_TYPE_SURVIVE										= 73,   ///> 绝地求生

	DESIGNATION_COM_TYPE_IDIP_ADD										= 500,   ///>潜规则，这类型不能配置只能服务器用。
	DESIGNATION_COM_TYPE_IDIP_DEL										= 501,   ///>潜规则，这类型不能配置只能服务器用。
	DESIGNATION_COM_TYPE_MAX, 
};


enum EVENT_MIX 
{
	EVENT_MIDAS_PAY_DEGREE = 1,												///>米大师消费额度
	EVENT_MIDAS_PAY_COUNT  = 2,
};

enum TASK_EVENT_ID
{
	TASK_EVENT_SET = 1,
	TASK_EVENT_ADD = 2,
	TASK_EVENT_DEL = 3,
	TASK_EVENT_ADD_TASKID_STEP = 4,		// 直接对某个任务加进度
	TASK_EVENT_HELP_RECORD = 5,			// 记录帮助信息
	TASK_EVENT_DAILY_TASK_REFRESH = 6,	// 日常任务刷新
	TASK_EVENT_CHARGE_BACK = 7,			// 回流充值返还
};

#endif
