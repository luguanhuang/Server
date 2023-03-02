#ifndef __MAILDEF_H__
#define __MAILDEF_H__


#define MAIL_ITEM				5
#define MAIL_EXPIRE_TIME		(30 * 24 * 3600)
#define MAX_MAIL_EXPIRE_TIME	(10 * 12 * 30 * 24 * 3600)

enum MailAttachment
{
	Mail_None,	///> �޸���
	Mail_Taken,	///> �и�������ȡ
	Mail_Given,	///> �и���δ��ȡ
};

enum MailType
{
	Mail_System,
	Mail_Good,
	Mail_Bad,
	Mail_Cost,
	Mail_Role,
};

enum ClientMailOpType
{
	Mail_Read,
	Mail_Delete,
	Mail_Reward,
	Mail_RewardAll,
};

enum MailConfId
{
	MailConf_WorldBoss			= 1,
	MailConf_PkWeekReward		= 2,
	MailConf_ArenaRankReward	= 3,
	MailConf_AuctionSale		= 4,
	MailConf_DEProgress			= 5,
	MailConf_GuildRankReward	= 7,
	MailConf_CardTeam			= 8,
	MailConf_CardMatch			= 9,
	MailConf_SkyCity			= 10,
	//��ά��
	MailConf_BANQUET			= 11,
	MailConf_BagFull			= 12,
	MailConf_PayMemDragonFlower = 14,
	MailConf_GuildAuct_Fail		= 13,
	MailConf_GuildAuct_BuyAuct_Success  = 18,
	MailConf_GuildAuct_BuyNow_Success  = 19,
	MailConf_GuildAuct_Profit_Normal   = 20,
	Mailconf_GuildAuct_Profit_Overflow = 21,
	MailConf_ResWar				= 22,
	MailConf_IDIP_COMPENSATION_AWARD = 23,
	MailConf_Guild_Donate = 25,
	MailConf_TeamCostBack = 26,
	MailConf_QQVipGift = 27,
	MailConf_QQSVipGift = 28,
	MailConf_TeamCostReward = 29,
	MailConf_PLANT = 30,
	MailConf_BANQUET_OVERTIMES		= 33,
	MailConf_HorseAward	= 34,
	MailConf_PokerTournament		= 35,
	MailConf_FASHIONTIMEOUT = 36,
	MailConf_PartnerDissolve = 37,
	MailConf_ReturnGuildTerr = 38,
	MailConf_GuildCastleJoin = 39,

	MailConf_PayMemberPetAward = 44,
	MailConf_PayMemberSpriteAward = 45,
	MailConf_LeagueTeamDissolve = 49,	// ����ս�ӽ�ɢ
	MailConf_LeagueTeamCreate = 50,		// ����ս�ӳ���
	MailConf_LeagueSeasonEndDissolve = 52,	// ������������ս�ӽ�ɢ
	MailConf_LeagueRank	= 53,			// ������������
	MailConf_LeagueCrossRank = 60,		// ���������������
	MailConf_PokerTournament_Refund	= 67, // �˿˴���û��ƥ�䵽���˷�

	MailConf_TeamDiamondCostCount = 68, //��ʯ�ͽ�����������
	MailConf_TeamCostHalf         = 69, //�ͽ����������
	MailConf_TeamCostNormal       = 87, //�ͽ���������
	MailConf_GuildModifyName      = 100,	//�������

	MailConf_RoleRename		= 101,	// �Ϸ���ɫ�����ʼ�
	MailConf_GuildRename	= 102,	// �Ϸ���������ʼ�

	MailConf_Military_UpRank		= 71,	// ��һ�δﵽ���׵Ľ���
	MailConf_Military_Award			= 72,	// ������������
	MailConf_SkyCraftRankReward		= 73,	// �����������������
	MailConf_SkyTeamDissolve		= 74,	// ���ս�ӽ�ɢ
	MailConf_SkyTeamDissolveSeasonEnd = 75, // ���ս������������ɢ
	MailConf_SkyTeamCreate			= 76,	// ���ս�ӳ���

	MailConf_ArenaStar_DianZan      = 83,
	MailConf_GIFTIB_THANKS			= 84,	//��л
	MailConf_GrowthFundCloseSoon = 85, //���𽫹ر�֪ͨ
	MailConf_DragonExp_TongGuan = 86, //��֮Զ��ͨ��

	MailConf_TrophyRankAward		= 88, // ��ҫ�ȼ�����
	MailConf_Abyss_Mail_Mon     = 103,	//�ڰ�Ů��ٵ����� ��1
	MailConf_Abyss_Mail_Tue     = 104,	
	MailConf_Abyss_Mail_Wed     = 105,
	MailConf_Abyss_Mail_Thu     = 106,
	MailConf_Abyss_Mail_Fri     = 107,
	MailConf_Abyss_Mail_Sat     = 108,
	MailConf_Abyss_Mail_Sun     = 109,

	MailConf_HeroReportPlayerBack	= 120,
	MailConf_HeroBeReport			= 121,

	MailConf_MobaBeReport			= 133,

	MailConf_MayhemPointAward = 130,
	MailConf_MayhemRankAward = 131,
	MailConf_MayhemJoinAward = 132,

	MailConf_DuckTicketsAward = 140, //��ѼΧ����Ʊ;

	MailConf_MarriageDivorce = 150, //���
	MailConf_WeddingThanks = 151,	// ����л��

	MailConf_WorldBossKill = 152,
	MailConf_AccountBackFlow = 153,
	MailConf_MarriageActivity = 154, //���

	MailConf_BattleField      = 155,


	MailConf_FuseCompensation = 156,
	MailConf_CompeteDragon = 157, //��������
	MailConf_ForgeCompensation = 159,
	MailConf_ConsumePre = 160, //��Ȩ֪ͨ;


	MailConf_DragonGuildModifyName   = 161,	// С�ֶӸ���
	MailConf_DragonGuildDismiss		 = 162,	// С�ֶӽ�ɢ
	MailConf_DragonGuileBeFired		 = 163,	// ����
	MailConf_DragonGuildLeaderDown	 = 164,	// ������ӳ�
	MailConf_DragonGuildLeaderUp	 = 165,	// ����Ϊ�ӳ�
	MailConf_PartnerCompensate		 = 166,	// ս��ϵͳ����

	MailConf_Rebate = 170, //���ѷ���;
	MailConf_GuildCastleFeats = 171,//�������ս��ѫ

	MailConf_StageBoxReward = 172,//�ؿ����佱��

	MailConf_CombineServerReward = 89, // �Ϸ������ʼ�

	MailConf_MSGiveItemFailed = 200,

	MailConf_ChargeBack = 173,	// ������ֵ����
	MailConf_ChargeBackLast = 174,	// ������ֵ���������һ��

	MailConf_Max,
};

#endif
