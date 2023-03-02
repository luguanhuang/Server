#ifndef __MAILDEF_H__
#define __MAILDEF_H__


#define MAIL_ITEM				5
#define MAIL_EXPIRE_TIME		(30 * 24 * 3600)
#define MAX_MAIL_EXPIRE_TIME	(10 * 12 * 30 * 24 * 3600)

enum MailAttachment
{
	Mail_None,	///> 无附件
	Mail_Taken,	///> 有附件已领取
	Mail_Given,	///> 有附件未领取
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
	//宴会奖励
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
	MailConf_LeagueTeamDissolve = 49,	// 联赛战队解散
	MailConf_LeagueTeamCreate = 50,		// 联赛战队成立
	MailConf_LeagueSeasonEndDissolve = 52,	// 联赛赛季结束战队解散
	MailConf_LeagueRank	= 53,			// 联赛排名奖励
	MailConf_LeagueCrossRank = 60,		// 联赛跨服排名奖励
	MailConf_PokerTournament_Refund	= 67, // 扑克大赛没有匹配到，退费

	MailConf_TeamDiamondCostCount = 68, //钻石赏金团收益达最大
	MailConf_TeamCostHalf         = 69, //赏金团收益减半
	MailConf_TeamCostNormal       = 87, //赏金团正常给
	MailConf_GuildModifyName      = 100,	//公会改名

	MailConf_RoleRename		= 101,	// 合服角色改名邮件
	MailConf_GuildRename	= 102,	// 合服工会改名邮件

	MailConf_Military_UpRank		= 71,	// 第一次达到军阶的奖励
	MailConf_Military_Award			= 72,	// 军阶排名奖励
	MailConf_SkyCraftRankReward		= 73,	// 天空争霸赛排名奖励
	MailConf_SkyTeamDissolve		= 74,	// 天空战队解散
	MailConf_SkyTeamDissolveSeasonEnd = 75, // 天空战队赛季结束解散
	MailConf_SkyTeamCreate			= 76,	// 天空战队成立

	MailConf_ArenaStar_DianZan      = 83,
	MailConf_GIFTIB_THANKS			= 84,	//答谢
	MailConf_GrowthFundCloseSoon = 85, //基金将关闭通知
	MailConf_DragonExp_TongGuan = 86, //龙之远征通关

	MailConf_TrophyRankAward		= 88, // 荣耀等级奖励
	MailConf_Abyss_Mail_Mon     = 103,	//黑暗女神官的来信 周1
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

	MailConf_DuckTicketsAward = 140, //疯鸭围城门票;

	MailConf_MarriageDivorce = 150, //离婚
	MailConf_WeddingThanks = 151,	// 结婚答谢礼

	MailConf_WorldBossKill = 152,
	MailConf_AccountBackFlow = 153,
	MailConf_MarriageActivity = 154, //结婚活动

	MailConf_BattleField      = 155,


	MailConf_FuseCompensation = 156,
	MailConf_CompeteDragon = 157, //竞技龙本
	MailConf_ForgeCompensation = 159,
	MailConf_ConsumePre = 160, //特权通知;


	MailConf_DragonGuildModifyName   = 161,	// 小分队改名
	MailConf_DragonGuildDismiss		 = 162,	// 小分队解散
	MailConf_DragonGuileBeFired		 = 163,	// 被踢
	MailConf_DragonGuildLeaderDown	 = 164,	// 被解除队长
	MailConf_DragonGuildLeaderUp	 = 165,	// 任命为队长
	MailConf_PartnerCompensate		 = 166,	// 战友系统补偿

	MailConf_Rebate = 170, //消费返利;
	MailConf_GuildCastleFeats = 171,//公会领地战功勋

	MailConf_StageBoxReward = 172,//关卡宝箱奖励

	MailConf_CombineServerReward = 89, // 合服补偿邮件

	MailConf_MSGiveItemFailed = 200,

	MailConf_ChargeBack = 173,	// 回流充值返利
	MailConf_ChargeBackLast = 174,	// 回流充值返利，最后一个

	MailConf_Max,
};

#endif
