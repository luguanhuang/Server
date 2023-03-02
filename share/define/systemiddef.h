#ifndef __SYSTEMIDDEF_H__
#define __SYSTEMIDDEF_H__

enum SystemId
{
	SYS_PET				  = 15,
	GuildSysId			  = 17,
	SYS_AUCTION			  = 22,		//拍卖行
	SysMailId			  = 27,
	DesignationID		  = 29, 
	SysFatigueId		  = 44,
	SYS_SUPERRISK		  = 49,
	DragonExpSysID		  = 50,
	PkId				  = 54,
	SYS_LIVEWATCH		  = 56,
	SYS_MENTOR			  = 75,		//师徒系统
	SYS_QQDIANJING		  = 80,		// 企鹅电竞
	SYS_EGame			  = 80,		// 企鹅电竞
	GuildBossId			  = 85,
	SYS_RESWAR			  = 88,
	TitleID				  = 94,
	SYS_MILITARY_RANK     = 98,		// 军衔系统

	SYS_PLATFORMSHARE_BOX = 102,	///>平台分享宝箱
	SYS_ARENA_STAR_ID     = 103,    // 竞技名人堂

	SmeltingId			  = 124,
	SYS_FASHION_DISPLAY   = 161,	//时装外显示
	SYS_FASHION_COMPOSE   = 162,    ///> 时装合成
	SYS_ITEM_COMPOSE	  = 170,	//合成升级物品

	CustomBattle_All	  = 220,
	CustomBattle_System   = 221,
	CustomBattle_Custom   = 222,
	WeekEndNest			  = 230,   //周末巢穴

	DesignationID_design  = 290, 
	DesignationID_achieve = 291, 

	ActivityRewardID	  = 351,
	SysCheckinId          = 353,
	SysDragonHelpReward	  = 355,
    SysGoalAward            = 357,

	XSys_Artifact_Recast  = 374,	//重铸
	XSys_Artifact_Fuse	  = 375,	//融合
	XSys_Artifact_Inscription = 376,//铭文

	SysPPTRewardId		  = 410,
	SysLevelRewardId      = 411,

	SysFlowerActiviyId    = 427,

	SYS_TEAM_NEST		  = 520,	//巢穴
	SysWorldBossId		  = 524,
	SysRankWorldBossId	  = 409,
	SYS_PVP				  = 528,
	SYS_BIGMELEESTART	  = 531,
	SYS_BIGMELEEEND		  = 532,
	SYS_BATTLEFIELD		  = 533,
	SYS_SURVIVE           = 534,   //绝地求生
	SYS_SHANGGU			  = 541,	//上古活动
	SYS_COMPETEDRAGON     = 542,  ///> 竞技龙本
	SYS_SKYCITY			  = 552,
	SYS_HORSE			  = 553,	//赛马的系统ID
    SYS_WEEKEND4V4        = 554,    //周末活动
	SYS_SKYARENAEND		  = 555,	//天空竞技场结束的系统id
	Welfare_DiamondCard	  = 561,
	Welfare_GiftBag		  = 562,
	SysItemFindBack		  = 565,
	SysGoldClick		  = 566,

	SYS_BACKFLOW_LEVELUP  = 586,	//回流等级直升
	SYS_BACKFLOW_TARGET   = 588,	//回归目标

	SysTaJieHelp		  = 590,

	SYS_FIRSTPASS		  = 601,
	SYS_WEEK_NEST_ID	  = 607,    // 每周巢穴
	SYS_FESTIVAL_ACTIVE	  = 609,
	SYS_IDIPNOTICE		  = 610,	//idip公告
	XSys_Pay			  = 652,

	////////////////
	SYS_PARTNER			  = 700,	// 战友系统ID
	SYS_PARTNER_LIVENESS  = 701,	// 战友活跃度商店
	SYS_MARRIAGE		  = 702,	// 结婚系统
	SYS_NPCFEELING        = 703,    // NPC好感度
	SYS_XINYUEVIP		  = 710,	//心悦
	SYS_CUSTOMSERVICE	  = 711,	//客服
	SYS_GAMEWEBSITE		  = 712,	//官网
	SYS_FORUM			  = 713,	//论坛
	SYS_PRIVILEGE		  = 714,	//vip特权
	SYS_OFFICIALACCOUNTS  = 715,	//公众号
	SYS_DEEPLINK		  = 716,	//抢福利
	SYS_MINICOMMUNITY	  = 718,	//微社区
	SYS_GCIOSLIVE		  = 721,	//IOS直播
	SYS_TVSTATION		  = 722,	//电视台
	SYS_BIOHELL			  = 771,	//生化地狱犬;
	SYS_DUCK			  = 772,	//疯鸭围城;


	SYS_GUILD_CHECKIN	  = 810,
	GuildFatigueId		  = 813,
	GuildCardSystem		  = 820,
	SYS_GUILD_AUDIO		  = 821,
	SYS_GUILD_GOBLIN      = 840,
	SYS_CDKEY			  = 841,	//CDKey
	SYS_OLDPLAYERREWARD   = 842,	//老玩家回馈
	SYS_OLDPLAYERDESIGNATION=843,	//老玩家称号
	SYS_IOSCOMMENT		    = 844,	//IOS好评
	SYS_VALENTINESDAY	    = 845,	//情人节送花
	SYS_GUILD_LADDER	    = 860,
	SYS_DAILY_TASK		    = 886,	// 每日任务
	SYS_520DAY				= 847,	// 520送花


	SYS_POKERTOURNAMENT     = 900,	// 扑克之王
	SYS_COLLECTITEMS        = 901,	// 集字活动
	SYS_WEEKLYTASK			= 904,	// 周任务
	SYS_PKTWO               = 921,  // 2v2天梯
	SYS_SPRITE_LOTTERY		= 932,	// 精灵抽奖
	SYS_SPRITE_SHOP			= 936,	// 精灵商店			
	SYS_LINK_SHARE		    = 942,	//链接分享
	SYS_APPSTORE_PRAISE     = 944,	//appstore点赞的
	SYS_GAMECOMMUNITY	    = 950,	//游戏社区
	SYS_GAMEHORDE		    = 951,	//兴趣部落
	SYS_FRIENDCIRCLE	    = 952,	//游戏圈
	SYS_QQVIP			    = 953,
	SYS_IBSHOPGIFT		    = 954,	//直购
	SYS_TEAMLEAGUE		    = 960,	// 战队联赛
	SYS_PROFESSION_CHANGE = 961,            ///> 变职业
	SYS_QUESTIONNAIRE	    = 962,	// 问卷	
	SYS_RESWAR_AWARD	= 963,				///>矿脉派奖
	SYS_LOGINACTIVITY   = 995,				///>登录活动
	SYS_FRIENDGIFT_SHARE    = 980,	// 好友排行分享
	SYS_GUILD_BIND_GROUP    = 981,	// 榜群
	SYS_PLATFORM_STARTPRIVILEGE = 982,// 平台启动特权
	SYS_PHOTO_SHARE				= 983,// 拍照分享
	SYS_IBSHOP_GIFT             = 985,// 直购
	//SYS_GUILDCAMP_PARTY			= 990, // 工会营地
	//SYS_GUILDCAMP_EXCHANGEITEM  = 991, //工会营地物品交换
	SYS_GUILD_CAMP_PARTY_NTF	= 992, // 工会营地通知
	SYS_BACKFLOW_MALL			= 993, // 回流神秘商店
	SYS_BACKFLOW_BUY            = 994,  ///> 回流购买

	SYS_DRAGONGUILD	= 996,				// 小分队
	SYS_DRAGONGUILD_SHOP = 997,         //小分队商店
	SYS_DRAGONGUILD_LIVENESS = 998,     //小分队活跃
	SYS_DRAGONGUILD_TASK = 999,         //小分队任务
	SYS_DRAGONGUILD_BINDGROUP = 1000,	//小分队绑定qq群

	MAX_SYSTEM_ID = 1024,
};

#endif