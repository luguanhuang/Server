#ifndef _H_TLog_Enum_H__
#define _H_TLog_Enum_H__

//TLog相关的枚举定义

//
//注: 下列是腾讯给出定义，不可随意修改

enum TGuildActType
{
	TX_GUILD_ACT_TYPE_1 = 1,	//创建
	TX_GUILD_ACT_TYPE_2 = 2,	//解散
	TX_GUILD_ACT_TYPE_3 = 3,	//升级
	TX_GUILD_ACT_TYPE_4 = 4,	//加入
	TX_GUILD_ACT_TYPE_5 = 5,	//退出
	TX_GUILD_ACT_TYPE_6 = 6,	//公会工资记录;
	TX_GUILD_ACT_TYPE_7	= 7,	//经验修改;
	TX_GUILD_ACT_TYPE_8 = 8,    //威望修改;
	TX_GUILD_ACT_TYPE_9	= 9,	//公会活跃度记录;
	TX_GUILD_ACT_TYPE_10 = 10,	//公会擂台战报名;
	TX_GUILD_ACT_TYPE_11 = 11,	//踢人;
};

enum TGuildDismissMode
{
	Tx_GuildDismiss_Self = 0, //主动解散;
	Tx_GuildDismiss_System = 1,//系统解散;
};

enum TGuildArenaFlowType
{
	TGuildArena_Type1 = 1, //鼓舞
	TGuildArena_Type2 = 2, //1v2
	TGuildArena_Type3 = 3, //6v6
};
//
enum TSpecialItemID
{
	TX_DragonScalesID = 85,	//水龙之鳞ID;
	TX_PandoraHeartID = 610,//潘多拉之心ID;
};


enum TAddOrReduce
{
	TX_ADD = 0,
	TX_REDUCE = 1,
};
enum TShopType
{
	TX_NORMAL_SHOP = 1,			//商城
};
enum TMoneyType
{
	TX_MONEY	= 0,			//金币
	TX_DIAMOND	= 1,			//钻石
	TX_DRAGON_COIN = 2,			//龙币
};
enum TBattleResult
{
	TX_BATTLE_FAIL		= 0,	//失败 修改一下保持与tx定义的接口一致
	TX_BATTLE_SUCCESS	= 1,	//成功
	TX_BATTLE_SWEEP		= 2,	//扫荡
	TX_BATTLE_DRAW		= 3,	//平局
	TX_BATTLE_RUNAWAY	= 4,	//逃跑

};

enum TItemEnhanceResult
{
	TX_ITEM_DEFAULT = 0,	//等级不变
	TX_ITEM_UP = 1,		//等级提升
	TX_ITEM_DOWN = 2,		//等级下降
	TX_ITEM_TRANSTER = 3,	//强化转移
};

enum TBattleType
{
	TX_BATTLE_PVE		= 0,	//单人游戏
	TX_BATTLE_PVP		= 1,	//对战游戏
	TX_BATTLE_OTHER		= 2,	//其他对局
};

enum TPlatID
{
	TX_PLAT_IOS			= 0,	//ios
	TX_PLAT_ANDROID		= 1,	//android
};

enum TSnsType
{
	TX_SNS_SHOWOFF			= 0,	//炫耀
	TX_SNS_INVITE			= 1,	//邀请
	TX_SNS_SEND_HEART		= 2,	//送心
	TX_SNS_RECEIVE_HEART	= 3,	//收取心
	TX_SNS_SEND_EMAIL		= 4,	//发邮件
	TX_SNS__RECEIVE_EMAIL	= 5,	//收邮件
	TX_SNS_SHARE			= 6,	//分享
	TX_SNS_OTHER			= 7,	//其他原因
};

enum AuctionType
{
	TX_AUCTION_SELL = 0, //上架物品
	TX_AUCTION_BUY = 1, //购买物品
};

enum GuildChallengeType
{
	TX_GUILD_CHALLENGE = 0, //工会挑战
	TX_GUILD_ARENA	= 1,	//工会擂台
};

enum GuildCardType
{
	TX_GUILD_CARD_START = 1,//开始
	TX_GUILD_CARD_CHANGE = 2,//换牌
	TX_GUILD_CARD_END = 3, //结束
};

enum TChatType
{
	TX_Chat_Mail = 0, //邮件
	TX_Chat_Private = 1,//私聊
	TX_Chat_World = 2,//世界
	TX_Chat_Main = 3,//主城
	TX_Chat_Guild = 4,//公会
	TX_Chat_Team = 5,//组队
	TX_Chat_Current = 6,//当前
};

enum TSpriteType
{
	TX_Sprite_Get = 0,				//获得
	TX_Sprite_Take = 1,				//分解
	TX_Sprite_InFight = 2,			//上阵
	TX_Sprite_OutFight = 3,			//下阵
	TX_Sprite_SetLeader = 4,		//设置队长
	TX_Sprite_CancelLeader = 5,		//被取消队长
	TX_Sprite_AddExp = 6,			//经验修改
	TX_Sprite_Awake = 7,			//觉醒
	TX_Sprite_AwakeReplace = 8,		//保留觉醒后
	TX_Sprite_EvolutionLevelUp = 9, //升星	
	TX_Sprite_Train = 10,			//培养
	TX_Sprite_ResetTrain = 11,		//重置培养
	TX_Sprite_Rebirth = 12,			//重生
};

enum TPetType
{
	TX_Pet_ExpandSeat = 0, //开启栏位
	TX_Pet_Get		= 1, //坐骑获得
	TX_Pet_Release  = 2, //流放
	TX_Pet_Feed		= 3, //喂养
	TX_Pet_Touch	= 4, //抚摸
	TX_Pet_Fellow	= 5, //乘骑
	TX_Pet_Fight	= 6, //激活
	TX_Pet_Down		= 7, //卸下
};

enum TGardenOpType
{
	TX_Garden_PlantPlay = 1,			//种植
	TX_Garden_FishPlay = 2,				//钓鱼
	TX_Garden_CookingPlay = 3,			//烹饪
	TX_Garden_BanquetPlay = 4,			//宴请
	TX_Garden_GuildPlantPlay = 5,		//公会种植
	TX_Garden_Visit	= 6,				//拜访
	TX_Garden_ActiveCookBook = 7,		//激活菜谱
	Tx_Garden_Cultivation = 8,			//培养
	Tx_Garden_OpenFarmland = 9,			//开地
	Tx_Garden_Steal = 10,				//偷
	Tx_Garden_ExpelSprite = 11,			//驱除小妖

};

enum TGuildAuctionType
{
	TX_GuildAuctionStart = 1,			//公会上架;
	TX_WorldAuctionStart = 2,			//流拍到世界;
	TX_GuildBidding		 = 3,			//公会竞价;
	TX_WorldBidding		 = 4,			//世界竞价;
	TX_GuildBuyNow		 = 5,			//公会中一口价购买;
	TX_WorldBuyNow		 = 6,			//世界中一口价购买;
	TX_MakeProfit        = 7,			//分红;
	TX_GuildBiddingDone  = 8,			//公会竞技成交;
	TX_WorldBiddingDone	 = 9,			//世界竞价成交;
};

enum EPkResultType
{
	TX_PkResult_Win = 0,	//胜利
	TX_PKResult_Lost = 1,	//失败
	TX_PKResult_Draw = 2,	//平局
};

enum ESceneStatusType
{
	TXEnterSceneType = 0, //进入场景
	TXLeaveSceneType = 1, //离开场景
	TXLikeSceneType  = 2, //观战中点赞;
};

enum EProfessionStatusType
{
	TX_Transfer_Profession = 0, //转职
	TX_Reset_Profession = 1,	//重置转职
};

enum TBlackListMode
{
	TX_BlackList_DoMode = 1,//拉黑
	TX_BlackList_UnDoMode = 2,//取消拉黑
	TX_AddFriend = 3,//加好友
	TX_DelFriend = 4,//解除好友
};
enum TEmblemType
{
	TX_Emblem_Identify = 1,//鉴定
	TX_Emblem_Smelt = 2,//洗炼
	TX_Emblem_Wear = 3,//穿上
	TX_Emblem_TakeOff = 4,//卸下
	Tx_Emblem_Logout = 5, //登出记录身上的纹章;
};

enum TItemSysType
{
	TX_Item_Smelt = 1,//洗炼
	TX_Item_Forge = 2,//锻造
	TX_Item_Enchant = 3,//附魔
	TX_Item_JadeEquip = 4,//龙玉镶嵌
	TX_Item_JadeOff = 5,//龙玉卸下
	TX_Item_JadeUp = 6,//龙玉升级
	TX_Item_ForgeReplace = 7,//锻造替换
	TX_Item_Fuse = 8,//装备进阶
	TX_Item_FuseBreak = 9,//进阶突破

};

enum TMailOpType
{
	Tx_Mail_Get = 1,//收到邮件;
	Tx_Mail_Reward = 2,//领取奖励;
};

enum TLeagueTeamType
{
	Tx_LeagueTeamType_Create = 1,//创建
	Tx_LeagueTeamType_Dissolve = 2, //解散
};

enum TArtifactOpType
{
	TX_Artifact_Wear = 1,//穿上
	TX_Artifact_TakeOff = 2,//卸下
	TX_Artifact_ComposeTake = 3,//合成消耗
	TX_Artifact_ComposeGet = 4, //合成获得
	Tx_Artifact_BodyLogout = 5, //登出时身上纹章;
	Tx_Artifact_Recast = 6,	//重铸;
	Tx_Artifact_Fuse = 7, //融合;
	Tx_Artifact_FuseOk = 8,//融合成功后;
	Tx_Artifact_FuseTake = 9,//融合消耗
	Tx_Artifact_Inscription = 10, //铭文镶嵌;
};
enum TCasteType
{
	Tx_CasteType0 = 0, //失败
	Tx_CasteType1 = 1, //成功未占领
	Tx_CasteType2 = 2, //成功且占领
};

enum TMentorFlowType
{
	Tx_MentorType0 = 0, //拜师
	Tx_MentorType1 = 1, //收徒
	Tx_MentorType2 = 2, //解除师徒(徒弟)
	Tx_MentorType3 = 3, //解除师徒（师父）
	Tx_MentorType4 = 4, //完成师徒任务
	Tx_MentorType5 = 5, //出师
};

enum TMarriageType
{
	TMarriageType1 = 1,		//发起结婚;
	TMarriageType2 = 2,		//结婚;
	TMarriageType3 = 3,		//发起离婚;
	TMarriageType4 = 4,		//取消离婚;
	TMarriageType5 = 5,		//离婚;
	TMarriageType6 = 6,		//发起婚宴;
};

enum TDragonGuildActType
{
	TX_Dragon_GUILD_ACT_TYPE_1 = 1,		//创建
	TX_Dragon_GUILD_ACT_TYPE_2 = 2,		//解散
	TX_Dragon_GUILD_ACT_TYPE_3 = 3,		//升级
	TX_Dragon_GUILD_ACT_TYPE_4 = 4,		//加入
	TX_Dragon_GUILD_ACT_TYPE_5 = 5,		//退出
	TX_Dragon_GUILD_ACT_TYPE_6	= 6,	//经验修改;
	TX_Dragon_GUILD_ACT_TYPE_7	= 7,	//小分队活跃度记录;
	TX_Dragon_GUILD_ACT_TYPE_8 = 8,		//踢人;
};

enum TPayScoreType
{
	TPayScoreType1 = 1,		//消费获得积分
	TPayScoreType2 = 2,		//消费恢复特权
	TPayScoreType3 = 3,		//降级;
};

enum TGroupChatType
{
	TGroupChatType1 = 1,	//创建群聊
	TGroupChatType2 = 2,	//加入群聊
	TGroupChatType3 = 3,	//自己退出群聊
	TGroupChatType4 = 4,	//被踢出群聊
	TGroupChatType5 = 5,	//解散群聊;
};

enum TWeekTaskType
{
	TWeekTaskType0 = 0, //接受任务;
	TWeekTaskType1 = 1, //刷新;
	TWeekTaskType2 = 2, //求助;
	TWeekTaskType3 = 3, //交付;
	TWeekTaskType4 = 4, //进度变化;
};

enum TDailyTaskReFreshFlowType
{
	TDailyTaskReFreshFlowType_Accept = 1,
	TDailyTaskReFreshFlowType_AskHelp = 2,
	TDailyTaskReFreshFlowType_Refuse = 3,
	TDailyTaskReFreshFlowType_Refresh = 4,
	TDailyTaskReFreshFlowType_BuyCount = 5,
};
#define TXLOG_EMBLEMATTRNUM 5 //单独定义，因为会影响tlog的列数,多定义用于扩展;
#define TXLOG_JADENUM 5 //单独定义，龙玉个数,多定义用于扩展
#define TXLOG_ITEMRANDOM_ATTRNUM 5 //装备随机属性个数;
#define TXLOG_ITEMFORGE_ATTRNUM 3 //装备锻造属性个数;
#define TXLOG_EMBLE_ITEMNUM 5 //纹章操作消耗道具材料的个数
#define TXLOG_ITEM_COSTNUM 10 //装备操作消耗材料的个数;
#define TXLOG_ITEMENCHANGCE_COSTNUM 10//装备强化消耗的材料个数;
#define TXLOG_ArtifactEffectNum 10 //龙器效果个数;
#define TXLOG_ArtifactAttrNum 5 //龙器属性个数;
#define TXLOG_SPRITE_ADDATTR_NUM 10 //精灵资质个数
#define TXLOG_SPRITE_ATTRNUM 10//精灵属性数;
#define TXLOG_SPRITE_EVOATTR_NUM 10 //精灵加成个数
#define TXLOG_ENCHANTNOT_ATTRNUM 40 //附魔未选择的属性个数
//
//注: 下列是自定义枚举
//

//(必填)货币流动一级原因
enum EMoneyFlowType
{
};

//(可选)货币流动二级原因
enum EMoneyFlowSubType
{
};

//((必填)道具流动一级原因
enum EItemFlowType
{
	ItemFlow_Task,			///> 任务

	ItemFlow_Stage,			///> 关卡

	ItemFlow_Skill,			///> 技能
	ItemFlow_Level,			///> level 

	ItemFlow_Equip,			///> 装备
	ItemFlow_Compose,		///> 合成
	ItemFlow_Decompose,		///> 分解
	ItemFlow_Smelt,			///> 洗炼
	ItemFlow_Emblem,		///> 纹章
	ItemFlow_Jade,			///> 龙玉

	ItemFlow_Pay,			///> pay
	ItemFlow_Shop,			///> 商店

	ItemFlow_Reward,		///> 运营活动

	ItemFlow_Lottory,		///> 抽奖
	ItemFlow_Fatigue,		///> 体力
	ItemFlow_Activity,		///> 活跃度
	ItemFlow_Checkin,		///> 签到
	ItemFlow_Achievement,	///> 成就
	ItemFlow_Fashion,		///> 时装
	ItemFlow_Flower,		///> 送花
	ItemFlow_TShow,			///> T台
	ItemFlow_Mail,			///> 邮件
	ItemFlow_FindBack,		///> 补偿
	ItemFlow_Buy,			///> 买 
	ItemFlow_GM,			///> GM
	ItemFlow_Guild,			///> 公会
	ItemFlow_GaoJiGu,		///> 家园
	ItemFlow_Pet,			///> 宠物
	ItemFlow_Camp,			///> camp
	ItemFlow_Auction,		///> acution
	ItemFlow_AchieveV2,		///> 新版成就
	ItemFlow_Chat,			///> 聊天【世界】
	ItemFlow_IDIP,			///> idip接口
	ItemFlow_QA,			///> 答题
	ItemFlow_DRAGON,		///> 龙本
	ItemFlow_Tower,			///> 黑暗神殿
	ItemFlow_Goddess,		///> 女神的试炼
	ItemFlow_Friend,		///> 好友
	ItemFlow_Title,			///> 头衔
	ItemFlow_FirstPass,		///> 首通
	ItemFlow_ItemFindBack,	///> 道具找回
	ItemFlow_BuyIBShop,		///> 购买IB道具
	ItemFlow_Atlas,         ///> 图鉴
	ItemFlow_Sprite,		///> 精灵
	ItemFlow_SuperRisk,		///> 大冒险
	ItemFlow_LevelSeal,		///> 封印系统
	ItemFlow_UseItem,		///> 使用道具
	ItemFlow_OpenSystem,	///> 系统开放
	ItemFlow_SpActivity,	///> 活动（开服活动，解封活动等）
	ItemFlow_Garden,		///> 家园
	ItemFlow_GuildAuction,  ///> 公会拍卖
	ItemFlow_GoldClick,		///> 摇钱树
	ItemFlow_MailBagFull,	///> 背包满了发邮件
	ItemFlow_TeamCost,		///> 赏金团
	ItemFlow_GuildInherit,	///> 公会传功

	ItemFlow_Rename,		///> 改名
	ItemFlow_Enchant,		///> 装备附魔
	ItemFlow_Partner,		///> 战友系统
	ItemFlow_PokerTournament,///> 扑克之王
	ItemFlow_GuildTerritory, ///> 公会领地
	ItemFlow_HeroBattle,	///> 英雄战场
	ItemFlow_Forge,			///> 装备锻造

	ItemFlow_ItemSell,		///> 物品出售
	ItemFlow_ChangePro,		///> 变职业

	ItemFlow_LeagueBattle,	///> 战队联赛
	ItemFlow_CombineServer, ///> 合服补偿
	ItemFlow_GiftIBItem,	///> 赠送IB道具
	ItemFlow_ArenaStar,     ///> 竞技名人堂
	ItemFlow_Military,		///> 军衔
	ItemFlow_GuildCampParty, ///> 公会营地派对
	ItemFlow_AbyssParty,	///>深渊派对
	ItemFlow_Artifact,		///> 龙器
	ItemFlow_ReturnSmeltStone, ///> 洗炼石返还
	ItemFlow_CustomBattle, ///> 全民PK 
	ItemFlow_ItemExpirationTime, ///> 物品时限过期
	ItemFlow_PlatShareResult,	/// > 平台分享
	ItemFlow_Team,               ///组队

    ItemFlow_WeekEnd4v4,               ///周末派对
	ItemFlow_BackFlowInit,	///> 回归服创角福利
	ItemFlow_Mobabattle,	///> 英雄峡谷
	ItemFlow_Personal,		///> 个人职业生涯
	ItemFlow_Marriage,		///> 结婚
	ItemFlow_Wedding,		///> 婚礼

	ItemFlow_BackFlowBuy,		///> 回流直购
	ItemFlow_DragonGuild,		///> 小分队
    ItemFlow_GoalAwards,		///> 目标奖励
	ItemFlow_NpcFeeling,        ///> npc好感度
    ItemFlow_CompeteDragon,		///> 竞技龙本

	ItemFlow_Survive,           ///> 绝地求生

	ItemFlow_BackFlowShop,		///> 回流活动神秘商店
	ItemFlow_BackFlow,			///> 回流
};

//(必填)道具流动二级原因
enum EItemFlowSubType
{
	////注意：： 新加类型只能依次往下加，不能打乱上面的数据id  （请加中文注释）
	ItemFlow_Task_Finish,			///> 任务 
	ItemFlow_Stage_Revive,			///> 关卡复活
	ItemFlow_Stage_Count,			///> 关卡次数
	ItemFlow_Stage_Chest,			///> 关卡宝箱
	ItemFlow_Stage_Enter,			///> 关卡进入
	ItemFlow_Stage_Supplement,		///> 关卡补给
	ItemFlow_Stage_TeamBox,			///> 关卡抽宝箱
	ItemFlow_Stage_Doodad,			///> 关卡掉落
	ItemFlow_Stage_Battle,			///> 关卡奖励
	ItemFlow_Stage_Abysss,			///> 深渊奖励
	ItemFlow_Stage_Nest,			///> 巢穴奖励
	ItemFlow_Stage_WorldBoss,		///> 世界boss鼓舞
	ItemFlow_Stage_GuildBoss,		///> 工会boss鼓舞
	ItemFlow_Stage_Arena,			///> 竞技场奖励
	ItemFlow_Stage_Pk,				///> 排位赛奖励
	ItemFlow_Stage_BossRush,		///> bossrush奖励
	ItemFlow_Stage_ExpTeam,			///> 无
	ItemFlow_Stage_Tower,			///> 黑暗神殿奖励
	ItemFlow_Stage_Goblin,			///> 小妖精奖励
	ItemFlow_Stage_Unknown,			///> 关卡其他奖励
	ItemFlow_Skill_Up,				///> 技能升级
	ItemFlow_Skill_Reset,			///> 技能重置
	ItemFlow_Level_Give,			///> 等级奖励
	ItemFlow_Equip_Enhance,			///> 装备强化
	ItemFlow_Decompose_Item,		///> 物品分解
	ItemFlow_Equip_Compose,			///> 装备合成
	ItemFlow_Jade_Compose,			///> 龙玉合成
	ItemFlow_Jade_OpenSlot,			///> 龙玉开孔
	ItemFlow_Jade_BodyCompose,		///> 龙玉合成
	ItemFlow_Jade_Equip,			///> 镶嵌龙玉
	ItemFlow_Jade_Equip_Replace,	///> 龙玉替换
	ItemFlow_Jade_TakeOff,			///> 龙玉脱
	ItemFlow_Emblem_LevelUp,		///> 纹章升级
	ItemFlow_Emblem_IdentifyCost,	///> 纹章鉴定
	ItemFlow_Emblem_Compose,		///> 纹章合成
	ItemFlow_Shop_Buy,				///> 商店买
	ItemFlow_Shop_Refresh,			///> 商店刷新
	ItemFlow_Pay_Give,				///> 付费
	ItemFlow_Lottory_Draw,			///> 抽奖
	ItemFlow_Checkin_Day,			///> 签到
	ItemFlow_Activity_Day,			///> 活跃度
	ItemFlow_Flower_Send,			///> 送花赠送
	ItemFlow_Flower_Get,			///> 送花得到
	ItemFlow_Flower_GetRankReward,	///> 送花排行奖励
	ItemFlow_Achievement_Give,		///> 成就奖励
	ItemFlow_Fashion_Compose,		///> 时装合成
	ItemFlow_Fashion_SkillPoint,	///> 时装技能点
	ItemFlow_Mail_Give,				///> 邮件给
	ItemFlow_FindBack_Exp,			///> 经验找回
	ItemFlow_TShow_Vote,			///> T台投票
	ItemFlow_Fatigue_Add,			///> 增加体力
	ItemFlow_Buy_Coin,				///> 购买金币
	ItemFlow_Buy_Fatigue,			///> 购买体力
	ItemFlow_Buy_DragonCoin,		///> 购买龙币
	ItemFlow_GM_Give,				///> GM给
	ItemFlow_GM_Clear,				///> GM清理
	ItemFlow_Reward_System,			///> 系统奖励
	ItemFlow_Reward_Online,			///> 在线奖励
	ItemFlow_Reward_NextDay,		///> 次日奖励
	ItemFlow_Reward_Days,			///> 登录天数奖励
	ItemFlow_Reward_Login,			///> 登录奖励
	ItemFlow_Guild_Create,			///> 工会创建
	ItemFlow_Guild_Card,			///> 工会扑克
	ItemFlow_Guild_Checkin,			///> 工会签到
	ItemFlow_Guild_Fatigue,			///> 工会体力
	ItemFlow_Guild_Bonus,			///> 工会红包
	ItemFlow_GaoJiGu_Fish,			///> 钓鱼
	ItemFlow_Pet_Feed,				///> 宠物喂养
	ItemFlow_Camp_Task,				///> 阵营任务
	ItemFlow_Auction_Take,			///> 拍卖行扣
	ItemFlow_Auction_Give,			///> 拍卖行给
	ItemFlow_AchieveV2_Achieve,		///> 成就奖励
	ItemFlow_AchieveV2_AchievePoint,///> 成就点数奖励
	ItemFlow_Stage_Pvp,				///> 保卫队长
	ItemFlow_Chat_World,			///> 世界聊天
	ItemFlow_IDIP_Oper,				///> IDIP
	ItemFlow_QA_AnswerRight,		///> 答题答对奖励
	ItemFlow_DRAGON_Enter,			///> 进入龙本
	ItemFlow_Stage_Sweep,           ///> 扫荡
	ItemFlow_Tower_Sweep,			///> 黑暗神殿
	ItemFlow_Goddess_Reward,		///> 女神的眼泪奖励
	ItemFlow_Guild_Dare_Reward,		///> 公会挑战奖励
	ItemFlow_NestBuy,				///> 巢穴次数购买
	ItemFlow_AbysssBuy,				///> 深渊次数购买
	ItemFlow_FriendGift,			///> 好友礼物
	ItemFlow_EnhanceTrans,			///> 强化转移
	ItemFlow_Expand_Seat,           ///> 扩展坐骑栏  
	ItemFlow_BuyPay,				///> 充值获得钻石
	ItemFlow_BuyAileen,				///> 充值购买艾琳每日礼包
	ItemFlow_BuyPayCard,			///> 充值购买周/月卡
	ItemFlow_BuyGrowthFund,			///> 充值购买成长基金
	ItemFlow_PayCardAward,			///> 领取周月卡奖励
	ItemFlow_TitleLevelUp,			///> 头衔升级
	ItemFlow_PayFirstAward,			///> 领取首充奖励
	ItemFlow_FirstPassRank,			///> 首通
	ItemFlow_FirstPassCommend,		///> 首通点赞
	ItemFlow_VipLevelGift,			///> 领取vip等级礼包
	ItemFlow_GrowthFundAward,		///> 领取成长基金奖励
	ItemFlow_ItemFindBackCost,		///> 道具找回
	ItemFlow_ItemFindBackGet,		///> 道具找回获得
	ItemFlow_BuyIBItemByDiamond,	///> 采用钻石购买IB道具
	ItemFlow_BuyIBItemByDragonCoin,	///> 采用龙玉购买IB道具
	ItemFlow_CreateAtlasTeam,	    ///> 图鉴系统激活组合
	ItemFlow_SpriteLevelUp,			///> 精灵升级
	ItemFlow_SpriteEvolutionLevelUp,///> 精灵升星
	ItemFlow_SpriteAwake,			///> 精灵觉醒
	ItemFlow_SpriteDecompose,		///> 精灵分解
	ItemFlow_SpriteLottery,			///> 精灵蛋抽奖
	ItemFlow_GaoJiGu_UseFish,		///> 使用鱼
	ItemFlow_SuperRiskDice,		    ///> 大冒险
	ItemFlow_LevelSeal_Exchange,	///> 封印货币兑换礼包
	ItemFlow_SuperRiskOpenBox,		///> 大冒险开宝箱
	ItemFlow_SuperRiskBattle,		///> 大冒险战斗
	ItemFlow_SuperRiskAccelerate,	///> 大冒险加速
	ItemFlow_SuperRiskBuy,			///> 大冒险购买
	ItemFlow_SuperRiskRebattle,		///> 大冒险再次挑战
	ItemFlow_StageDragonExp,        ///> 龙之远征
	ItemFlow_SmeltEquip,			///> 装备洗炼
	ItemFlow_SmeltEmblem,			///> 纹章洗炼
	ItemFlow_Tower_RefreshSweep,	///> 黑暗神殿刷新扫荡结果
	ItemFlow_Tower_FirstPass,		///> 黑暗神殿首通
	ItemFlow_BuyArenaCost,			///> 购买竞技场次数
	ItemFlow_ClearArenaCD,			///> 清除竞技场CD
	ItemFlow_ResetProfession,		///> 重置职业
	ItemFlow_Buy_BlueBird,			///> 购买蓝鸟

	ItemFlow_UsePeck,				///> 使用礼包
	ItemFlow_CircleDraw,			///> 圆盘抽奖
	ItemFlow_UsePetBorn,			///> 使用宠物蛋
	ItemFlow_UseSceneCountTicket,	///> 使用入场券
	ItemFlow_OpenSystemGive,		///> 开放系统给 
	ItemFlow_SpActivity_GetReward,	///> 活动领取任务奖励
	ItemFlow_SpActivity_BigPrize,	///> 活动领取大奖
	ItemFlow_GuildCardMatch,		///> 工会卡牌大赛
	ItemFlow_Auction_GetBack,       ///> 拍卖行取回
	ItemFlow_LevelSealSelfGit,		///> 封印个人勋章奖励

	ItemFlow_Garden_PlantSeed,		///> 家园种植
	ItemFlow_Garden_Fish,			///> 家园钓鱼
	ItemFlow_Garden_Cooking,		///> 家园烹饪
	ItemFlow_Garden_Banquet,		///> 家园

	ItemFlow_Guild_Wage,			///> 公会工资

	ItemFlow_GuildAuct_Take,        ///> 公会拍卖

	ItemFlow_GoldClickGive,			///> 摇钱树
	ItemFlow_GoldClickTake,			///> 摇钱树

	ItemFlow_UsePandora,			///> 打开潘多拉之心

	ItemFlow_GuildAuct_Give,        ///> 公会拍卖

	ItemFlow_Donate,				///> 公会捐献
	
	ItemFlow_TeamCostTake,			///> 赏金团
	ItemFlow_TeamCostGive,			///> 赏金团
	ItemFlow_GuildInheritGive,		///> 公会传功

	ItemFlow_RenameCost,			///> 改名消耗
	ItemFlow_GuildBossAward,		///> 公会boss奖励
	ItemFlow_GuildArenaAward,		///> 公会擂台战奖励
	ItemFlow_GuildRankdAward,		///> 公会排名奖励
	ItemFlow_GuildResWarAward,		///> 公会矿脉战奖励
	ItemFlow_GuildCampRankAward,	///> 公会大冒险排名奖励
	ItemFlow_SkyCityAward,			///> 天空竞技场奖励
	ItemFlow_GuildCardAward,		///> 公会扑克奖励
	ItemFlow_FlowerWeekRankAward,	///> 鲜花排行奖励
	ItemFlow_GuildLadderAward,		///> 公会天梯赛奖励
	ItemFlow_QAAward,				///> 答题排行奖励
	ItemFlow_WorldBossAward,		///> 世界boss奖励
	ItemFlow_PkWeekRankAward,		///> 天梯赛周排行奖励
	ItemFlow_PlatFriendGift,		///> 平台好友gift
	ItemFlow_DragonExpRankAward,	///> 龙之远征排名奖励
	ItemFlow_DonateMemberItem,		///> 公会捐献
	ItemFlow_GuildAuctProfit,		///> 公会拍卖分红
	ItemFlow_GuildAuctFailBack,		///> 公会竞价失败退回
	ItemFlow_TeamCostAward,			///> 赏金团每日奖励
	ItemFlow_QQVip,					///> 手Q会员
	ItemFlow_PayMember,				///> 充值会员特权
	ItemFlow_TeamCostBack,			///> 赏金团返还
	ItemFlow_PetSkillBook,			///> 使用坐骑技能书
	ItemFlow_Enchant_Take,			///> 装备附魔消耗
	ItemFlow_MakePartner,			///> 结拜战友关系 
	ItemFlow_WeekNest,              ///> 每周巢穴
	ItemFlow_PartnerChest,			///> 战友活跃度宝箱
	ItemFlow_PokerTournamentAward,		///> 扑克之王奖励
	ItemFlow_PartnerShop,			///> 战友商店
	ItemFlow_PokerTournamentCost,	///> 扑克之王报名消耗
	ItemFlow_MentorComplete,		///> 师徒出师
	ItemFlow_GuildTerrChall,		///> 公会领地宣战
	ItemFlow_GuildTerrJoin,         ///> 公会领地参与
	
	ItemFlow_HeroBattle_Reward,		///> 英雄战场胜利奖励与每日参与奖励
	ItemFlow_HeroBattle_WeekReward,	///> 英雄战场每周胜场奖励
	ItemFlow_HeroBattle_BuyHero,	///> 英雄战场买英雄
	ItemFlow_SpActivity_Collect,	///> 完成收集活动扣物品
	ItemFlow_Desig,					///> 成就称号
	ItemFlow_Forge_Take,			///> 装备锻造消耗
	ItemFlow_Exchange_Dragoncoin,   ///> 钻石不足时，兑换指定数量的龙币

	ItemFlow_ItemSell_Out,			///> 物品出售
	ItemFlow_ItemSell_In,			///> 物品出售
	ItemFlow_Fashion_TimeOut,		///> 时装过期

	ItemFlow_ChangePro_Cost,		///> 变职业
	ItemFlow_ChangePro_ItemTake,	///> 变职业
	ItemFlow_ChangePro_ItemGive,	///> 变职业

	ItemFlow_SpriteTrain,			///> 精灵培养
	ItemFlow_SpriteResetTrain,		///> 精灵重置培养

	ItemFlow_LeagueBattleRace,		///> 联赛积分赛

	ItemFlow_SkillTwo,				///> 同步获取技能点2
	ItemFlow_PayReward,             ///> 支付礼包

	ItemFlow_MentorTask,	        ///> 师徒任务
	ItemFlow_FlowerValentinesDayRankAward,	///> 鲜花情人节排行奖励

	ItemFlow_Compose_Common,		///> 合成物品——通用

	ItemFlow_Team_UseTicket,        ///> 组队用门票

	ItemFlow_Stage_ActivityOne,    ///> 活动关卡1
	ItemFlow_Stage_ActivityTwo,    ///> 活动关卡2
	ItemFlow_Stage_ActivityThree,  ///> 活动关卡3

	ItemFlow_SpActivityPartner_GetReward,	///> 战友日活动领取任务奖励
	ItemFlow_SpActivityBackFlow_GetReward,	///> 回流活动领取任务奖励
	ItemFlow_SpActivityArgenta_GetReward,	///> 塔姐的福利领取任务奖励
	ItemFlow_SpActivityArgenta_GetDaily,	///> 塔姐的福利领取每日福利

	ItemFlow_WorldBoss_GuildAttr,		///> 世界boss公会鼓舞

	ItemFlow_HeroBattle_HeroTicket,		///> 使用英雄战场体验券

	ItemFlow_HeroBattle_HeroTicket_GiveReward,	///> 已拥有英雄，使用英雄战场体验券给奖励
	ItemFlow_CombineServer_Reward,		///> 合服补偿
	ItemFlow_BreakAtlas	,				///> 分解图鉴
	ItemFlow_LoginActivity,				///登录活动

	ItemFlow_Flower520DayRankAward,	///> 鲜花520排行奖励


	ItemFlow_FirstAchieveRank,		///> 军衔升级奖励
	ItemFlow_MilitaryRankAward,		///> 军衔月排行奖励

	ItemFlow_IBItemCostMoney,		///> IB直购扣费
	ItemFlow_IBItemGiveItem,		///> IB直购送道具
	ItemFlow_ArenaStar_DianZan,     ///> 竞技名人堂点赞
	ItemFlow_ABYSS_PARTY,			///> 深渊派对
	ItemFlow_Military_Exchange,		///> 军衔变化
	ItemFlow_GuildCampParty_RandomItem, ///> 公会派对抽奖物品
	ItemFlow_GuildCampParty_ExchangeItem, ///> 公会派对交换物品
	ItemFlow_AbyssPartyTick,				  ///> 深渊
	ItemFlow_ArtifactCompose_Take,	///> 龙器合成消耗
	ItemFlow_ArtifactCompose_Give,	///> 龙器合成给予
	ItemFlow_BuyExtraEmblemSlot,	///> 购买纹章孔
	ItemFlow_IdipXinYueRPay = 231,		///> 心悦大R代充,这个值不能改，否则会影响心悦充值;
	ItemFlow_AbyssLoginMail,			///>深渊派对登录奖励
	ItemFlow_UseItem_BagExpand,		///> 背包扩容
	ItemFlow_ReturnSmeltStone_Equip,	///> 装备洗炼石返还
	ItemFlow_ReturnSmeltStone_Emblem,	///> 纹章洗炼石返还
	ItemFlow_Traja,						///> 塔姐道具

	ItemFlow_TrajaAward,				///> 塔姐奖励

	ItemFlow_CustomBattle_Invalid, ///> 全民PK 
	ItemFlow_CustomBattle_Join, ///> 全民PK 加入
	ItemFlow_CustomBattle_ClearCD, ///> 全民PK 加入
	ItemFlow_CustomBattle_Create, ///> 全民PK 创建
	ItemFlow_CustomBattle_RewardSystem, ///> 全民PK 奖励
	ItemFlow_CustomBattle_RewardCustom, ///> 全民PK 奖励
	ItemFlow_ItemExpirationTime_Take,	///> 物品时限过期扣除

	ItemFlow_IdipExchange, ///> idip物品兑换;
	ItemFlow_PlatShare_Award,			///> 平台分享奖励

	ItemFlow_FlowerActivityDayRankAward,	///> 鲜花活动排行奖励

	ItemFlow_FASHION_ACTIVATE_COLOR,	///> 时装，发型换颜色
	ItemFlow_Team_DragonHelp,          ///> 龙本助战

    ItemFlow_WeekEnd4v4_MONSTERFIGHT,               ///周末派对怪物大乱斗
    ItemFlow_WeekEnd4v4_GHOSTACTION,               ///周末派对幽灵行动
    ItemFlow_WeekEnd4v4_LIVECHALLENGE,               ///周末派对生存大挑战
    ItemFlow_WeekEnd4v4_CRAZYBOMB,               ///周末派对疯狂炸弹人
    ItemFlow_WeekEnd4v4_HORSERACING,               ///周末派对团队赛马
	ItemFlow_BackFlowInit_Equip,		///> 回归服创角装备福利
	ItemFlow_BackFlowInit_Emblem,		///> 回归服创角纹章福利
	ItemFlow_BackFlowInit_Jade,			///> 回归服创角龙玉福利

	ItemFlow_MayhemPointAward, //大乱斗积分奖励
	ItemFlow_MayhemRankAward, //大乱都排行奖励
	ItemFlow_MayhemJoinAward, //大乱斗积分奖励

	ItemFlow_ArtifactComposeMulti_Take,		///> 龙器一键合成消耗
	ItemFlow_ArtifactComposeMulti_Give,		///> 龙器一键合成给予
	ItemFlow_MobaBattle_Reward,				///> 英雄峡谷每局胜场奖励
	ItemFlow_MobaBattle_WeekReward,			///> 英雄峡谷每周胜场奖励
	ItemFlow_Sprite_BuySpriteEgg_Take,		///> 购买精灵药水送精灵蛋花费
	ItemFlow_Sprite_BuySpriteEgg_Give,		///> 购买精灵药水送精灵蛋给予

	ItemFlow_DragonExp_TongGuan,            ///> 龙之远征通关
	ItemFlow_Personal_Honour_Award,			///> 个人职业生涯荣耀奖励
	ItemFlow_AncientTimes,					///> 上古活动奖励
	ItemFlow_DuckTickets,					///> 疯鸭门票
	ItemFlow_DuckAward,						///> 疯鸭副本奖励
	ItemFlow_DuckDayTickets,			///>疯鸭每日门票

	ItemFlow_Marriage_Marry,		///> 结婚
	ItemFlow_Marriage_Divorce,		///> 离婚

	ItemFlow_BioHell,						///> 生化地狱犬结算奖励;
	ItemFlow_WeddingFlow,					///> 结婚撒花
	ItemFlow_WeddingFireworks,				///> 结婚放烟花
	ItemFlow_WeddingCandy,					///> 结婚捡喜糖
	ItemFlow_MarriageLiveness,				///> 结婚活跃度
	ItemFlow_WeddingThanks,					///> 婚宴答谢

    ItemFlow_WeekEnd4v4_DUCK,               ///> 周末派对钩鸭子
	ItemFlow_WorldBossKillAward,			///> 世界boss击杀参与奖励

	ItemFlow_Activity_Coin,					///> 活跃度，回归活跃币
	ItemFlow_ArtifactRecastCost,			///> 龙器重铸
	ItemFlow_ArtifactFuse,					///> 龙器融合
	ItemFlow_ArtifactFuseUseStone,			///> 龙器融合使用提炼石
	ItemFLow_ArtifactInscription,			///> 龙器使用技能铭文

	ItemFlow_BackFlowBuy_Take,				///> 回流直购扣
	ItemFlow_BackFlowBuy_Give,				///> 回流直购给
	ItemFlow_SpriteRebirth,					///> 精灵重生

	ItemFlow_TransOhter,					///> 变别人

	ItemFlow_SpActivity_WeekNest_GetReward,	///> 周末星巢穴活动领取任务奖励

	ItemFlow_Equip_Upgrade,					///> 装备升级

	ItemFlow_DragonGuildCreate,				///> 小分队创建
	ItemFlow_DragonGuildTaskChest,          ///> 小分队领取任务奖励
	ItemFlow_DragonGuildShop,				///> 小分队商店
	ItemFlow_PartnerCompenstate,			///> 战友系统补偿
    ItemFlow_GoalAwards_GetReward,			///> 目标奖励中领取奖励
	ItemFlow_WeeklyTaskRefresh,				///> 周任务刷新
	ItemFlow_BattleFieldWeekPoint,			///> 战场周积分
	ItemFlow_Equip_FuseAddExp,				///> 装备熔铸加进度
	ItemFlow_Equip_FuseBreak,				///> 装备熔铸突破
	ItemFlow_NpcFeeling_GiveNpc,            ///> NPC好感度 赠送给npc
	ItemFlow_TaskTurnOver,					///> 任务上缴物品
	ItemFlow_Equip_FuseCompensation,		///> 装备熔铸老玩家补偿
    ItemFlow_CompeteDragonThrough,			///> 竞技龙本
	ItemFlow_NpcFeeling_EX_Role2Npc,		///> NPC好感度 交换 玩家给NPC
	ItemFlow_NpcFeeling_EX_Npc2Role,        ///> NPC好感度 交换 NPC给玩家
	ItemFlow_NpcFeeling_NpcReturn,          ///> NPC好感度 NPC回赠
	ItemFlow_NpcFeeling_NpcBuyCount,        ///> NPC好感度 购买次数
	ItemFlow_BuyDailyTaskRefreshCount,		///> 购买日常任务刷新次数

	ItemFlow_Survive_RankRward,             ///> 绝地求生排名奖励
	ItemFlow_Survive_DayReward,             ///> 绝地求生日常奖励
	ItemFlow_ConsumeRebate,					///>开服消费返利

	ItemFlow_Stage_PeerTake,				///> 放大镜
	ItemFlow_Stage_BoxGive,					///> 抽宝箱
	ItemFlow_ActivatePre,					///> 付费常态特权激活;
	ItemFlow_GuildTerrFeats,				///> 公会领地功勋邮件奖励

	ItemFlow_KingBackGive,                  ///> 和王者归来组队奖励

	ItemFlow_Material,						///> 材料
	ItemFlow_BackFlow_GetTreasure,			///> 回流活动领取宝箱
	ItemFlow_BackFlowShop_Diamond,			///> 回流活动神秘商店用钻石
	ItemFlow_BackFlowShop_DragonCoin,		///> 回流活动神秘商店用龙币	
	ItemFlow_BackFlowShop_Give,				///> 回流活动神秘商店购买给物品
	ItemFlow_GuildBonus,					///> 红包道具
	ItemFlow_BackFlowChargeBack,			///> 回流历史充值返利
	ItemFlow_Equip_ForgeCompensation,		///> 装备锻造老玩家补偿
	ItemFlow_AncientTimesTake,				///> 上古活动扣除上次积分
////> 注意：： 新加类型只能依次往下加，不能打乱上面的数据id  （请加中文注释）
};


//(必填)经验流动一级原因
enum EPlayerExpFlowType
{
};

//(必填)经验流动二级原因
enum EPlayerExpFlowSubType
{
};

//(必填)运营活动类型
enum EActivityType
{
};


//抽奖类型，任务类型(已有)


//任务状态
enum ETaskState
{
	TASK_STATE_ACCEPTED			= 0,		//接受
	TASK_STATE_COMPLETED		= 1,		//完成
};

#define MAX_PAY_BILL_COUNT 100 //缓存订单数;
#endif
