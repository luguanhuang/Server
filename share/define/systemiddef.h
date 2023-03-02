#ifndef __SYSTEMIDDEF_H__
#define __SYSTEMIDDEF_H__

enum SystemId
{
	SYS_PET				  = 15,
	GuildSysId			  = 17,
	SYS_AUCTION			  = 22,		//������
	SysMailId			  = 27,
	DesignationID		  = 29, 
	SysFatigueId		  = 44,
	SYS_SUPERRISK		  = 49,
	DragonExpSysID		  = 50,
	PkId				  = 54,
	SYS_LIVEWATCH		  = 56,
	SYS_MENTOR			  = 75,		//ʦͽϵͳ
	SYS_QQDIANJING		  = 80,		// ���羺
	SYS_EGame			  = 80,		// ���羺
	GuildBossId			  = 85,
	SYS_RESWAR			  = 88,
	TitleID				  = 94,
	SYS_MILITARY_RANK     = 98,		// ����ϵͳ

	SYS_PLATFORMSHARE_BOX = 102,	///>ƽ̨������
	SYS_ARENA_STAR_ID     = 103,    // ����������

	SmeltingId			  = 124,
	SYS_FASHION_DISPLAY   = 161,	//ʱװ����ʾ
	SYS_FASHION_COMPOSE   = 162,    ///> ʱװ�ϳ�
	SYS_ITEM_COMPOSE	  = 170,	//�ϳ�������Ʒ

	CustomBattle_All	  = 220,
	CustomBattle_System   = 221,
	CustomBattle_Custom   = 222,
	WeekEndNest			  = 230,   //��ĩ��Ѩ

	DesignationID_design  = 290, 
	DesignationID_achieve = 291, 

	ActivityRewardID	  = 351,
	SysCheckinId          = 353,
	SysDragonHelpReward	  = 355,
    SysGoalAward            = 357,

	XSys_Artifact_Recast  = 374,	//����
	XSys_Artifact_Fuse	  = 375,	//�ں�
	XSys_Artifact_Inscription = 376,//����

	SysPPTRewardId		  = 410,
	SysLevelRewardId      = 411,

	SysFlowerActiviyId    = 427,

	SYS_TEAM_NEST		  = 520,	//��Ѩ
	SysWorldBossId		  = 524,
	SysRankWorldBossId	  = 409,
	SYS_PVP				  = 528,
	SYS_BIGMELEESTART	  = 531,
	SYS_BIGMELEEEND		  = 532,
	SYS_BATTLEFIELD		  = 533,
	SYS_SURVIVE           = 534,   //��������
	SYS_SHANGGU			  = 541,	//�ϹŻ
	SYS_COMPETEDRAGON     = 542,  ///> ��������
	SYS_SKYCITY			  = 552,
	SYS_HORSE			  = 553,	//�����ϵͳID
    SYS_WEEKEND4V4        = 554,    //��ĩ�
	SYS_SKYARENAEND		  = 555,	//��վ�����������ϵͳid
	Welfare_DiamondCard	  = 561,
	Welfare_GiftBag		  = 562,
	SysItemFindBack		  = 565,
	SysGoldClick		  = 566,

	SYS_BACKFLOW_LEVELUP  = 586,	//�����ȼ�ֱ��
	SYS_BACKFLOW_TARGET   = 588,	//�ع�Ŀ��

	SysTaJieHelp		  = 590,

	SYS_FIRSTPASS		  = 601,
	SYS_WEEK_NEST_ID	  = 607,    // ÿ�ܳ�Ѩ
	SYS_FESTIVAL_ACTIVE	  = 609,
	SYS_IDIPNOTICE		  = 610,	//idip����
	XSys_Pay			  = 652,

	////////////////
	SYS_PARTNER			  = 700,	// ս��ϵͳID
	SYS_PARTNER_LIVENESS  = 701,	// ս�ѻ�Ծ���̵�
	SYS_MARRIAGE		  = 702,	// ���ϵͳ
	SYS_NPCFEELING        = 703,    // NPC�øж�
	SYS_XINYUEVIP		  = 710,	//����
	SYS_CUSTOMSERVICE	  = 711,	//�ͷ�
	SYS_GAMEWEBSITE		  = 712,	//����
	SYS_FORUM			  = 713,	//��̳
	SYS_PRIVILEGE		  = 714,	//vip��Ȩ
	SYS_OFFICIALACCOUNTS  = 715,	//���ں�
	SYS_DEEPLINK		  = 716,	//������
	SYS_MINICOMMUNITY	  = 718,	//΢����
	SYS_GCIOSLIVE		  = 721,	//IOSֱ��
	SYS_TVSTATION		  = 722,	//����̨
	SYS_BIOHELL			  = 771,	//��������Ȯ;
	SYS_DUCK			  = 772,	//��ѼΧ��;


	SYS_GUILD_CHECKIN	  = 810,
	GuildFatigueId		  = 813,
	GuildCardSystem		  = 820,
	SYS_GUILD_AUDIO		  = 821,
	SYS_GUILD_GOBLIN      = 840,
	SYS_CDKEY			  = 841,	//CDKey
	SYS_OLDPLAYERREWARD   = 842,	//����һ���
	SYS_OLDPLAYERDESIGNATION=843,	//����ҳƺ�
	SYS_IOSCOMMENT		    = 844,	//IOS����
	SYS_VALENTINESDAY	    = 845,	//���˽��ͻ�
	SYS_GUILD_LADDER	    = 860,
	SYS_DAILY_TASK		    = 886,	// ÿ������
	SYS_520DAY				= 847,	// 520�ͻ�


	SYS_POKERTOURNAMENT     = 900,	// �˿�֮��
	SYS_COLLECTITEMS        = 901,	// ���ֻ
	SYS_WEEKLYTASK			= 904,	// ������
	SYS_PKTWO               = 921,  // 2v2����
	SYS_SPRITE_LOTTERY		= 932,	// ����齱
	SYS_SPRITE_SHOP			= 936,	// �����̵�			
	SYS_LINK_SHARE		    = 942,	//���ӷ���
	SYS_APPSTORE_PRAISE     = 944,	//appstore���޵�
	SYS_GAMECOMMUNITY	    = 950,	//��Ϸ����
	SYS_GAMEHORDE		    = 951,	//��Ȥ����
	SYS_FRIENDCIRCLE	    = 952,	//��ϷȦ
	SYS_QQVIP			    = 953,
	SYS_IBSHOPGIFT		    = 954,	//ֱ��
	SYS_TEAMLEAGUE		    = 960,	// ս������
	SYS_PROFESSION_CHANGE = 961,            ///> ��ְҵ
	SYS_QUESTIONNAIRE	    = 962,	// �ʾ�	
	SYS_RESWAR_AWARD	= 963,				///>�����ɽ�
	SYS_LOGINACTIVITY   = 995,				///>��¼�
	SYS_FRIENDGIFT_SHARE    = 980,	// �������з���
	SYS_GUILD_BIND_GROUP    = 981,	// ��Ⱥ
	SYS_PLATFORM_STARTPRIVILEGE = 982,// ƽ̨������Ȩ
	SYS_PHOTO_SHARE				= 983,// ���շ���
	SYS_IBSHOP_GIFT             = 985,// ֱ��
	//SYS_GUILDCAMP_PARTY			= 990, // ����Ӫ��
	//SYS_GUILDCAMP_EXCHANGEITEM  = 991, //����Ӫ����Ʒ����
	SYS_GUILD_CAMP_PARTY_NTF	= 992, // ����Ӫ��֪ͨ
	SYS_BACKFLOW_MALL			= 993, // ���������̵�
	SYS_BACKFLOW_BUY            = 994,  ///> ��������

	SYS_DRAGONGUILD	= 996,				// С�ֶ�
	SYS_DRAGONGUILD_SHOP = 997,         //С�ֶ��̵�
	SYS_DRAGONGUILD_LIVENESS = 998,     //С�ֶӻ�Ծ
	SYS_DRAGONGUILD_TASK = 999,         //С�ֶ�����
	SYS_DRAGONGUILD_BINDGROUP = 1000,	//С�ֶӰ�qqȺ

	MAX_SYSTEM_ID = 1024,
};

#endif