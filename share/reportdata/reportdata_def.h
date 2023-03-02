#ifndef __REPORTDATA_DEF_H__
#define __REPORTDATA_DEF_H__

enum EnumBcover
{
	Tx_Reportdata_BcoverRank = 0, //排行榜;
	Tx_Reportdata_BcoverNormal = 1,//非排行榜;
};
enum EnumNestType
{
	NestHell = 0,//地狱犬巢穴;
	NestLionScorpion = 1, //狮蝎巢穴;
	NestTianQi = 2, //天启
	NestDaZhuJiao = 3,//大主教
	DragonSea = 4,//海龙
	DragonGreen = 5,//绿龙

};
enum EnumNestLevel
{
	NestLevel0 = 0,
	NestLevel1 = 1,
	NestLevel2 = 2,
	NestLevel3 = 3,
	NestLevel4 = 4,
};

enum EnumReportDataType
{
	Tx_Reportdata_Level = 1,		//等级;
	Tx_Reportdata_Money = 2,		//钻石;
	Tx_Reportdata_PkPoint = 3,		//天梯赛积分;
	Tx_Reportdata_LoginTime = 8,	//最近登录时间;
	Tx_Reportdata_PlatType = 12,	//平台类型;
	Tx_Reportdata_PowerPoint = 17,	//战力;
	Tx_Reportdata_TowerFloor = 19,	//黑暗神殿已爬的层数;
	Tx_Reportdata_Flower = 21,		//历史魅力值;
	Tx_Reportdata_FlowerRank = 22,	//历史魅力值排行;
	Tx_Reportdata_PkTotalNum = 23,	//天梯赛总局数;
	Tx_Reportdata_PkWinRate = 24,	//天梯赛胜率;
	Tx_Reportdata_CreateTime =25,	//注册时间;
	Tx_Reportdata_AreaID = 26,		//大区ID;
	Tx_Reportdata_ServerID = 27,	//服务器ID;
	Tx_Reportdata_RoleID = 28,		//角色ID;
	Tx_Reportdata_RoleName = 29,	//角色名称;
	Tx_Reportdata_GuildID = 30,		//所属公会ID;
	Tx_Reportdata_JoinGuildIDTime = 31,//加入公会时间;
	Tx_Reportdata_ProfessionID = 35,//职业ID;
	Tx_Reportdata_TotalPay = 43,	//累计充值金额;
	Tx_Reportdata_PayNum = 44,		//单笔充值金额;
	Tx_Reportdata_VipLevel = 45,	//vip等级;
	Tx_Reportdata_PayTime = 46,		//充值时间;
	Tx_Reportdata_GuildName = 301,	//公会名称;
	Tx_Reportdata_GuildLv = 302,	//公会等级;
	Tx_Reportdata_GuildCreateTime = 306,//公会创建时间;
	Tx_Reportdata_GuildDismissTime = 307,//公会解散时间;
	Tx_Reportdata_GuildMemberChange = 309,//成员变动;
	Tx_Reportdata_GuildPosChange = 311,//公会人员身份变更;
	Tx_Reportdata_GuildBindQQ = 312,	//公会绑定的qq群;
	Tx_Reportdata_GuildBindQQTime = 313, //绑群的时间;
	Tx_Reportdata_PlayNestHell = 1100,	//地狱犬巢穴参与;
	Tx_Reportdata_PlayNestLionScorpion = 1101,//狮蝎巢穴参与;
	Tx_Reportdata_FirstPassNestHell = 1102,//首次地狱犬通关;
	Tx_Reportdata_FirstPassNestLionScorpion = 1103,//首次狮蝎通关;
	Tx_Reportdata_WinNestHell = 1104,//地狱犬巢穴通关;
	Tx_Reportdata_WinNestLionScorpion = 1105, //狮蝎巢穴通关;
	Tx_Reportdata_PlayNestTianQi = 1106, //天启巢穴参与;
	Tx_Reportdata_PlayNestDaZhuJiao = 1107, //大主教巢穴参与;
	Tx_Reportdata_FirstPassNestTianQi = 1108, //首次天启通关;
	Tx_Reportdata_FirstPassNestDaZhuJiao = 1109, //首次大主教通关;
	Tx_Reportdata_WinNestTianQi = 1110, //天启巢穴通关;
	Tx_Reportdata_WinNestDaZhuJiao = 1111,//大主教通关;
	Tx_Reportdata_PlayDragonSea = 1112, //海龙本参与;
	Tx_Reportdata_PlayDragonGreen = 1113, //绿龙本参与;
	Tx_Reportdata_FirstPassDragonSea = 1114,//首次海龙本通关;
	Tx_Reportdata_FirstPassDragonGreen = 1115,//首次绿龙本通关;
	Tx_Reportdata_WinDragonSea = 1116,//海龙本通关;
	Tx_Reportdata_WinDragonGreen = 1117,//绿龙本通关;

	Tx_Reportdata_CardsGroup1Num = 1300,//图鉴阿尔特里亚激活数量;
	Tx_Reportdata_CardsGroup2Num = 1301,//图鉴塞尼亚激活数量;
	Tx_Reportdata_CardsGroupAddAttr = 1302,//图鉴属性总加成;
	Tx_Reportdata_WeekActiveDays = 1400,//用户登录即上报，数值做累加1-7，不登录则保持前一天数值不变，周末底清零;
	Tx_Reportdata_WeekEndLogin = 1401,//用户登录即上报，周一至周五为0，周六或周日登录上报即报1;
	Tx_Reportdata_2DayWithin20Mins = 1402,//连续3天活跃时长小于20分钟
	Tx_Reportdata_GuildRank = 1403,//所在公会排名;
	Tx_Reportdata_PKDayTimes   = 1404,  // 天梯赛每日次数
	Tx_Reportdata_PKWeekTimes  = 1405,  //近七天天梯赛参与场数
	Tx_Reportdata_HeroBattleTimes  = 1406, //当天英雄战场参与场数
	Tx_Reportdata_PvpDayTimes = 1407,	// 当天保护队长参与场数
	Tx_Reportdata_NestFastPassTime = 1409, //竞技巢穴最快通关时间;
	TX_Reportdata_GuildScoreLv = 1410, //所在公会表现等级
	Tx_Reportdata_NestDayTimes = 1411, //当天竞技巢穴参与场数
	Tx_Reportdata_AbysssDayTimes = 1412, //当天参与深渊副本次数
	Tx_Reportdata_EndlessAbyss = 1413, //当天参与无尽深渊次数
	Tx_Reportdata_GuildInheritTimes = 1414, //当天参与公会传承
	Tx_Reportdata_BossRushDayRank = 1415,//当天通关BOSSRUSH关数
	Tx_Reportdata_HeroBattleKD = 1416,//英雄战场KD(击杀/死亡，当死亡为0按1算）
	Tx_Reportdata_HeroResoult  = 1417, //英雄战场结果（胜利1/失败0）
	Tx_Reportdata_QuanMinPk = 1418, //全民pk赛
	Tx_Reportdata_BuyFashion = 1419, //购买时装
	Tx_Reportdata_SFashion = 1420, //拥有s级时装
	Tx_Reportdata_AllFashion = 1421, //拥有整套时装（0代表A级、1代表S级）
	Tx_Reportdata_AbyssParty = 1422, //当天参与龙魂禁地副本次数;

	Tx_Reportdata_RecommondPoint = 1426, //当前等级战斗力减去当前等级推荐战斗力(每周第一次登录时上报);
	Tx_Reportdata_MobaDayCount = 1427, //当天英雄峡谷参与次数;
	Tx_Reportdata_MobaResult = 1428, //英雄峡谷结果（胜利1/失败0）;
	Tx_Reportdata_MobaKD = 1429, //英雄峡谷KD(击杀/死亡，当死亡为0是按1算）;
	Tx_Reportdata_SuperRiskDayCount = 1430, //当天参与大陆冒险次数;
	Tx_Reportdata_BuyIbShopCount = 1431,//购买钻石商店道具次数;
	Tx_Reportdata_FriendCount = 1432, //添加好友个数;
	Tx_Reportdata_MobaTotalCount = 1433, //参与英雄峡谷次数（历史总参与次数）;
	Tx_Reportdata_MobaWinCount = 1434, //英雄峡谷胜场次数（历史总参与次数）;
	Tx_Reportdata_PokerCount = 1435, //参与小丑扑克次数（历史总参与次数）;
	Tx_Reportdata_HorseMatchCount = 1436, //参与全民赛马次数（历史总参与次数）;
	Tx_Reportdata_SimpleDragonPass = 1437, //简单龙本通过;
	Tx_Reportdata_ComposeCount = 1438, //时装合成参与次数;
	Tx_Reportdata_ComposeSuccessCount = 1439,	//时装合成成功次数;
	Tx_Reportdata_ComposeFailCount = 1440, //时装合成失败次数;
	Tx_Reportdata_ComposeSuccessSCount = 1441, //合成S级时装的成功次数;
	Tx_Reportdata_ComposeSuccessACount = 1442, //合成A级时装的成功次数;

	Tx_Reportdata_PkResult = 1502,		//天梯赛结果;
	Tx_Reportdata_PkDailyTime = 1504,	//当天天梯赛累计游戏时长;
	Tx_Reportdata_PkRank = 1505,		//天梯赛排名(每次打完后上报);
	Tx_Reportdata_PvpWinRate = 1601,	//保护队长胜率;
	Tx_Reportdata_PvpResult = 1602,		//保护队长结果;
	Tx_Reportdata_PvpTotalNum = 1603,	//保护队长参与总场数;
	Tx_Reportdata_PvpDailyTime = 1604,	//当天保护队长累计游戏时长;


	Tx_Reportdata_DailyOnlineTime = 6000, //当天累计在线时间时长;

};

#endif