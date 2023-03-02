#ifndef _H_TLog_Enum_H__
#define _H_TLog_Enum_H__

//TLog��ص�ö�ٶ���

//
//ע: ��������Ѷ�������壬���������޸�

enum TGuildActType
{
	TX_GUILD_ACT_TYPE_1 = 1,	//����
	TX_GUILD_ACT_TYPE_2 = 2,	//��ɢ
	TX_GUILD_ACT_TYPE_3 = 3,	//����
	TX_GUILD_ACT_TYPE_4 = 4,	//����
	TX_GUILD_ACT_TYPE_5 = 5,	//�˳�
	TX_GUILD_ACT_TYPE_6 = 6,	//���Ṥ�ʼ�¼;
	TX_GUILD_ACT_TYPE_7	= 7,	//�����޸�;
	TX_GUILD_ACT_TYPE_8 = 8,    //�����޸�;
	TX_GUILD_ACT_TYPE_9	= 9,	//�����Ծ�ȼ�¼;
	TX_GUILD_ACT_TYPE_10 = 10,	//������̨ս����;
	TX_GUILD_ACT_TYPE_11 = 11,	//����;
};

enum TGuildDismissMode
{
	Tx_GuildDismiss_Self = 0, //������ɢ;
	Tx_GuildDismiss_System = 1,//ϵͳ��ɢ;
};

enum TGuildArenaFlowType
{
	TGuildArena_Type1 = 1, //����
	TGuildArena_Type2 = 2, //1v2
	TGuildArena_Type3 = 3, //6v6
};
//
enum TSpecialItemID
{
	TX_DragonScalesID = 85,	//ˮ��֮��ID;
	TX_PandoraHeartID = 610,//�˶���֮��ID;
};


enum TAddOrReduce
{
	TX_ADD = 0,
	TX_REDUCE = 1,
};
enum TShopType
{
	TX_NORMAL_SHOP = 1,			//�̳�
};
enum TMoneyType
{
	TX_MONEY	= 0,			//���
	TX_DIAMOND	= 1,			//��ʯ
	TX_DRAGON_COIN = 2,			//����
};
enum TBattleResult
{
	TX_BATTLE_FAIL		= 0,	//ʧ�� �޸�һ�±�����tx����Ľӿ�һ��
	TX_BATTLE_SUCCESS	= 1,	//�ɹ�
	TX_BATTLE_SWEEP		= 2,	//ɨ��
	TX_BATTLE_DRAW		= 3,	//ƽ��
	TX_BATTLE_RUNAWAY	= 4,	//����

};

enum TItemEnhanceResult
{
	TX_ITEM_DEFAULT = 0,	//�ȼ�����
	TX_ITEM_UP = 1,		//�ȼ�����
	TX_ITEM_DOWN = 2,		//�ȼ��½�
	TX_ITEM_TRANSTER = 3,	//ǿ��ת��
};

enum TBattleType
{
	TX_BATTLE_PVE		= 0,	//������Ϸ
	TX_BATTLE_PVP		= 1,	//��ս��Ϸ
	TX_BATTLE_OTHER		= 2,	//�����Ծ�
};

enum TPlatID
{
	TX_PLAT_IOS			= 0,	//ios
	TX_PLAT_ANDROID		= 1,	//android
};

enum TSnsType
{
	TX_SNS_SHOWOFF			= 0,	//��ҫ
	TX_SNS_INVITE			= 1,	//����
	TX_SNS_SEND_HEART		= 2,	//����
	TX_SNS_RECEIVE_HEART	= 3,	//��ȡ��
	TX_SNS_SEND_EMAIL		= 4,	//���ʼ�
	TX_SNS__RECEIVE_EMAIL	= 5,	//���ʼ�
	TX_SNS_SHARE			= 6,	//����
	TX_SNS_OTHER			= 7,	//����ԭ��
};

enum AuctionType
{
	TX_AUCTION_SELL = 0, //�ϼ���Ʒ
	TX_AUCTION_BUY = 1, //������Ʒ
};

enum GuildChallengeType
{
	TX_GUILD_CHALLENGE = 0, //������ս
	TX_GUILD_ARENA	= 1,	//������̨
};

enum GuildCardType
{
	TX_GUILD_CARD_START = 1,//��ʼ
	TX_GUILD_CARD_CHANGE = 2,//����
	TX_GUILD_CARD_END = 3, //����
};

enum TChatType
{
	TX_Chat_Mail = 0, //�ʼ�
	TX_Chat_Private = 1,//˽��
	TX_Chat_World = 2,//����
	TX_Chat_Main = 3,//����
	TX_Chat_Guild = 4,//����
	TX_Chat_Team = 5,//���
	TX_Chat_Current = 6,//��ǰ
};

enum TSpriteType
{
	TX_Sprite_Get = 0,				//���
	TX_Sprite_Take = 1,				//�ֽ�
	TX_Sprite_InFight = 2,			//����
	TX_Sprite_OutFight = 3,			//����
	TX_Sprite_SetLeader = 4,		//���öӳ�
	TX_Sprite_CancelLeader = 5,		//��ȡ���ӳ�
	TX_Sprite_AddExp = 6,			//�����޸�
	TX_Sprite_Awake = 7,			//����
	TX_Sprite_AwakeReplace = 8,		//�������Ѻ�
	TX_Sprite_EvolutionLevelUp = 9, //����	
	TX_Sprite_Train = 10,			//����
	TX_Sprite_ResetTrain = 11,		//��������
	TX_Sprite_Rebirth = 12,			//����
};

enum TPetType
{
	TX_Pet_ExpandSeat = 0, //������λ
	TX_Pet_Get		= 1, //������
	TX_Pet_Release  = 2, //����
	TX_Pet_Feed		= 3, //ι��
	TX_Pet_Touch	= 4, //����
	TX_Pet_Fellow	= 5, //����
	TX_Pet_Fight	= 6, //����
	TX_Pet_Down		= 7, //ж��
};

enum TGardenOpType
{
	TX_Garden_PlantPlay = 1,			//��ֲ
	TX_Garden_FishPlay = 2,				//����
	TX_Garden_CookingPlay = 3,			//���
	TX_Garden_BanquetPlay = 4,			//����
	TX_Garden_GuildPlantPlay = 5,		//������ֲ
	TX_Garden_Visit	= 6,				//�ݷ�
	TX_Garden_ActiveCookBook = 7,		//�������
	Tx_Garden_Cultivation = 8,			//����
	Tx_Garden_OpenFarmland = 9,			//����
	Tx_Garden_Steal = 10,				//͵
	Tx_Garden_ExpelSprite = 11,			//����С��

};

enum TGuildAuctionType
{
	TX_GuildAuctionStart = 1,			//�����ϼ�;
	TX_WorldAuctionStart = 2,			//���ĵ�����;
	TX_GuildBidding		 = 3,			//���Ὰ��;
	TX_WorldBidding		 = 4,			//���羺��;
	TX_GuildBuyNow		 = 5,			//������һ�ڼ۹���;
	TX_WorldBuyNow		 = 6,			//������һ�ڼ۹���;
	TX_MakeProfit        = 7,			//�ֺ�;
	TX_GuildBiddingDone  = 8,			//���Ὰ���ɽ�;
	TX_WorldBiddingDone	 = 9,			//���羺�۳ɽ�;
};

enum EPkResultType
{
	TX_PkResult_Win = 0,	//ʤ��
	TX_PKResult_Lost = 1,	//ʧ��
	TX_PKResult_Draw = 2,	//ƽ��
};

enum ESceneStatusType
{
	TXEnterSceneType = 0, //���볡��
	TXLeaveSceneType = 1, //�뿪����
	TXLikeSceneType  = 2, //��ս�е���;
};

enum EProfessionStatusType
{
	TX_Transfer_Profession = 0, //תְ
	TX_Reset_Profession = 1,	//����תְ
};

enum TBlackListMode
{
	TX_BlackList_DoMode = 1,//����
	TX_BlackList_UnDoMode = 2,//ȡ������
	TX_AddFriend = 3,//�Ӻ���
	TX_DelFriend = 4,//�������
};
enum TEmblemType
{
	TX_Emblem_Identify = 1,//����
	TX_Emblem_Smelt = 2,//ϴ��
	TX_Emblem_Wear = 3,//����
	TX_Emblem_TakeOff = 4,//ж��
	Tx_Emblem_Logout = 5, //�ǳ���¼���ϵ�����;
};

enum TItemSysType
{
	TX_Item_Smelt = 1,//ϴ��
	TX_Item_Forge = 2,//����
	TX_Item_Enchant = 3,//��ħ
	TX_Item_JadeEquip = 4,//������Ƕ
	TX_Item_JadeOff = 5,//����ж��
	TX_Item_JadeUp = 6,//��������
	TX_Item_ForgeReplace = 7,//�����滻
	TX_Item_Fuse = 8,//װ������
	TX_Item_FuseBreak = 9,//����ͻ��

};

enum TMailOpType
{
	Tx_Mail_Get = 1,//�յ��ʼ�;
	Tx_Mail_Reward = 2,//��ȡ����;
};

enum TLeagueTeamType
{
	Tx_LeagueTeamType_Create = 1,//����
	Tx_LeagueTeamType_Dissolve = 2, //��ɢ
};

enum TArtifactOpType
{
	TX_Artifact_Wear = 1,//����
	TX_Artifact_TakeOff = 2,//ж��
	TX_Artifact_ComposeTake = 3,//�ϳ�����
	TX_Artifact_ComposeGet = 4, //�ϳɻ��
	Tx_Artifact_BodyLogout = 5, //�ǳ�ʱ��������;
	Tx_Artifact_Recast = 6,	//����;
	Tx_Artifact_Fuse = 7, //�ں�;
	Tx_Artifact_FuseOk = 8,//�ںϳɹ���;
	Tx_Artifact_FuseTake = 9,//�ں�����
	Tx_Artifact_Inscription = 10, //������Ƕ;
};
enum TCasteType
{
	Tx_CasteType0 = 0, //ʧ��
	Tx_CasteType1 = 1, //�ɹ�δռ��
	Tx_CasteType2 = 2, //�ɹ���ռ��
};

enum TMentorFlowType
{
	Tx_MentorType0 = 0, //��ʦ
	Tx_MentorType1 = 1, //��ͽ
	Tx_MentorType2 = 2, //���ʦͽ(ͽ��)
	Tx_MentorType3 = 3, //���ʦͽ��ʦ����
	Tx_MentorType4 = 4, //���ʦͽ����
	Tx_MentorType5 = 5, //��ʦ
};

enum TMarriageType
{
	TMarriageType1 = 1,		//������;
	TMarriageType2 = 2,		//���;
	TMarriageType3 = 3,		//�������;
	TMarriageType4 = 4,		//ȡ�����;
	TMarriageType5 = 5,		//���;
	TMarriageType6 = 6,		//�������;
};

enum TDragonGuildActType
{
	TX_Dragon_GUILD_ACT_TYPE_1 = 1,		//����
	TX_Dragon_GUILD_ACT_TYPE_2 = 2,		//��ɢ
	TX_Dragon_GUILD_ACT_TYPE_3 = 3,		//����
	TX_Dragon_GUILD_ACT_TYPE_4 = 4,		//����
	TX_Dragon_GUILD_ACT_TYPE_5 = 5,		//�˳�
	TX_Dragon_GUILD_ACT_TYPE_6	= 6,	//�����޸�;
	TX_Dragon_GUILD_ACT_TYPE_7	= 7,	//С�ֶӻ�Ծ�ȼ�¼;
	TX_Dragon_GUILD_ACT_TYPE_8 = 8,		//����;
};

enum TPayScoreType
{
	TPayScoreType1 = 1,		//���ѻ�û���
	TPayScoreType2 = 2,		//���ѻָ���Ȩ
	TPayScoreType3 = 3,		//����;
};

enum TGroupChatType
{
	TGroupChatType1 = 1,	//����Ⱥ��
	TGroupChatType2 = 2,	//����Ⱥ��
	TGroupChatType3 = 3,	//�Լ��˳�Ⱥ��
	TGroupChatType4 = 4,	//���߳�Ⱥ��
	TGroupChatType5 = 5,	//��ɢȺ��;
};

enum TWeekTaskType
{
	TWeekTaskType0 = 0, //��������;
	TWeekTaskType1 = 1, //ˢ��;
	TWeekTaskType2 = 2, //����;
	TWeekTaskType3 = 3, //����;
	TWeekTaskType4 = 4, //���ȱ仯;
};

enum TDailyTaskReFreshFlowType
{
	TDailyTaskReFreshFlowType_Accept = 1,
	TDailyTaskReFreshFlowType_AskHelp = 2,
	TDailyTaskReFreshFlowType_Refuse = 3,
	TDailyTaskReFreshFlowType_Refresh = 4,
	TDailyTaskReFreshFlowType_BuyCount = 5,
};
#define TXLOG_EMBLEMATTRNUM 5 //�������壬��Ϊ��Ӱ��tlog������,�ඨ��������չ;
#define TXLOG_JADENUM 5 //�������壬�������,�ඨ��������չ
#define TXLOG_ITEMRANDOM_ATTRNUM 5 //װ��������Ը���;
#define TXLOG_ITEMFORGE_ATTRNUM 3 //װ���������Ը���;
#define TXLOG_EMBLE_ITEMNUM 5 //���²������ĵ��߲��ϵĸ���
#define TXLOG_ITEM_COSTNUM 10 //װ���������Ĳ��ϵĸ���;
#define TXLOG_ITEMENCHANGCE_COSTNUM 10//װ��ǿ�����ĵĲ��ϸ���;
#define TXLOG_ArtifactEffectNum 10 //����Ч������;
#define TXLOG_ArtifactAttrNum 5 //�������Ը���;
#define TXLOG_SPRITE_ADDATTR_NUM 10 //�������ʸ���
#define TXLOG_SPRITE_ATTRNUM 10//����������;
#define TXLOG_SPRITE_EVOATTR_NUM 10 //����ӳɸ���
#define TXLOG_ENCHANTNOT_ATTRNUM 40 //��ħδѡ������Ը���
//
//ע: �������Զ���ö��
//

//(����)��������һ��ԭ��
enum EMoneyFlowType
{
};

//(��ѡ)������������ԭ��
enum EMoneyFlowSubType
{
};

//((����)��������һ��ԭ��
enum EItemFlowType
{
	ItemFlow_Task,			///> ����

	ItemFlow_Stage,			///> �ؿ�

	ItemFlow_Skill,			///> ����
	ItemFlow_Level,			///> level 

	ItemFlow_Equip,			///> װ��
	ItemFlow_Compose,		///> �ϳ�
	ItemFlow_Decompose,		///> �ֽ�
	ItemFlow_Smelt,			///> ϴ��
	ItemFlow_Emblem,		///> ����
	ItemFlow_Jade,			///> ����

	ItemFlow_Pay,			///> pay
	ItemFlow_Shop,			///> �̵�

	ItemFlow_Reward,		///> ��Ӫ�

	ItemFlow_Lottory,		///> �齱
	ItemFlow_Fatigue,		///> ����
	ItemFlow_Activity,		///> ��Ծ��
	ItemFlow_Checkin,		///> ǩ��
	ItemFlow_Achievement,	///> �ɾ�
	ItemFlow_Fashion,		///> ʱװ
	ItemFlow_Flower,		///> �ͻ�
	ItemFlow_TShow,			///> T̨
	ItemFlow_Mail,			///> �ʼ�
	ItemFlow_FindBack,		///> ����
	ItemFlow_Buy,			///> �� 
	ItemFlow_GM,			///> GM
	ItemFlow_Guild,			///> ����
	ItemFlow_GaoJiGu,		///> ��԰
	ItemFlow_Pet,			///> ����
	ItemFlow_Camp,			///> camp
	ItemFlow_Auction,		///> acution
	ItemFlow_AchieveV2,		///> �°�ɾ�
	ItemFlow_Chat,			///> ���졾���硿
	ItemFlow_IDIP,			///> idip�ӿ�
	ItemFlow_QA,			///> ����
	ItemFlow_DRAGON,		///> ����
	ItemFlow_Tower,			///> �ڰ����
	ItemFlow_Goddess,		///> Ů�������
	ItemFlow_Friend,		///> ����
	ItemFlow_Title,			///> ͷ��
	ItemFlow_FirstPass,		///> ��ͨ
	ItemFlow_ItemFindBack,	///> �����һ�
	ItemFlow_BuyIBShop,		///> ����IB����
	ItemFlow_Atlas,         ///> ͼ��
	ItemFlow_Sprite,		///> ����
	ItemFlow_SuperRisk,		///> ��ð��
	ItemFlow_LevelSeal,		///> ��ӡϵͳ
	ItemFlow_UseItem,		///> ʹ�õ���
	ItemFlow_OpenSystem,	///> ϵͳ����
	ItemFlow_SpActivity,	///> ��������������ȣ�
	ItemFlow_Garden,		///> ��԰
	ItemFlow_GuildAuction,  ///> ��������
	ItemFlow_GoldClick,		///> ҡǮ��
	ItemFlow_MailBagFull,	///> �������˷��ʼ�
	ItemFlow_TeamCost,		///> �ͽ���
	ItemFlow_GuildInherit,	///> ���ᴫ��

	ItemFlow_Rename,		///> ����
	ItemFlow_Enchant,		///> װ����ħ
	ItemFlow_Partner,		///> ս��ϵͳ
	ItemFlow_PokerTournament,///> �˿�֮��
	ItemFlow_GuildTerritory, ///> �������
	ItemFlow_HeroBattle,	///> Ӣ��ս��
	ItemFlow_Forge,			///> װ������

	ItemFlow_ItemSell,		///> ��Ʒ����
	ItemFlow_ChangePro,		///> ��ְҵ

	ItemFlow_LeagueBattle,	///> ս������
	ItemFlow_CombineServer, ///> �Ϸ�����
	ItemFlow_GiftIBItem,	///> ����IB����
	ItemFlow_ArenaStar,     ///> ����������
	ItemFlow_Military,		///> ����
	ItemFlow_GuildCampParty, ///> ����Ӫ���ɶ�
	ItemFlow_AbyssParty,	///>��Ԩ�ɶ�
	ItemFlow_Artifact,		///> ����
	ItemFlow_ReturnSmeltStone, ///> ϴ��ʯ����
	ItemFlow_CustomBattle, ///> ȫ��PK 
	ItemFlow_ItemExpirationTime, ///> ��Ʒʱ�޹���
	ItemFlow_PlatShareResult,	/// > ƽ̨����
	ItemFlow_Team,               ///���

    ItemFlow_WeekEnd4v4,               ///��ĩ�ɶ�
	ItemFlow_BackFlowInit,	///> �ع�����Ǹ���
	ItemFlow_Mobabattle,	///> Ӣ��Ͽ��
	ItemFlow_Personal,		///> ����ְҵ����
	ItemFlow_Marriage,		///> ���
	ItemFlow_Wedding,		///> ����

	ItemFlow_BackFlowBuy,		///> ����ֱ��
	ItemFlow_DragonGuild,		///> С�ֶ�
    ItemFlow_GoalAwards,		///> Ŀ�꽱��
	ItemFlow_NpcFeeling,        ///> npc�øж�
    ItemFlow_CompeteDragon,		///> ��������

	ItemFlow_Survive,           ///> ��������

	ItemFlow_BackFlowShop,		///> ����������̵�
	ItemFlow_BackFlow,			///> ����
};

//(����)������������ԭ��
enum EItemFlowSubType
{
	////ע�⣺�� �¼�����ֻ���������¼ӣ����ܴ������������id  ���������ע�ͣ�
	ItemFlow_Task_Finish,			///> ���� 
	ItemFlow_Stage_Revive,			///> �ؿ�����
	ItemFlow_Stage_Count,			///> �ؿ�����
	ItemFlow_Stage_Chest,			///> �ؿ�����
	ItemFlow_Stage_Enter,			///> �ؿ�����
	ItemFlow_Stage_Supplement,		///> �ؿ�����
	ItemFlow_Stage_TeamBox,			///> �ؿ��鱦��
	ItemFlow_Stage_Doodad,			///> �ؿ�����
	ItemFlow_Stage_Battle,			///> �ؿ�����
	ItemFlow_Stage_Abysss,			///> ��Ԩ����
	ItemFlow_Stage_Nest,			///> ��Ѩ����
	ItemFlow_Stage_WorldBoss,		///> ����boss����
	ItemFlow_Stage_GuildBoss,		///> ����boss����
	ItemFlow_Stage_Arena,			///> ����������
	ItemFlow_Stage_Pk,				///> ��λ������
	ItemFlow_Stage_BossRush,		///> bossrush����
	ItemFlow_Stage_ExpTeam,			///> ��
	ItemFlow_Stage_Tower,			///> �ڰ�����
	ItemFlow_Stage_Goblin,			///> С��������
	ItemFlow_Stage_Unknown,			///> �ؿ���������
	ItemFlow_Skill_Up,				///> ��������
	ItemFlow_Skill_Reset,			///> ��������
	ItemFlow_Level_Give,			///> �ȼ�����
	ItemFlow_Equip_Enhance,			///> װ��ǿ��
	ItemFlow_Decompose_Item,		///> ��Ʒ�ֽ�
	ItemFlow_Equip_Compose,			///> װ���ϳ�
	ItemFlow_Jade_Compose,			///> ����ϳ�
	ItemFlow_Jade_OpenSlot,			///> ���񿪿�
	ItemFlow_Jade_BodyCompose,		///> ����ϳ�
	ItemFlow_Jade_Equip,			///> ��Ƕ����
	ItemFlow_Jade_Equip_Replace,	///> �����滻
	ItemFlow_Jade_TakeOff,			///> ������
	ItemFlow_Emblem_LevelUp,		///> ��������
	ItemFlow_Emblem_IdentifyCost,	///> ���¼���
	ItemFlow_Emblem_Compose,		///> ���ºϳ�
	ItemFlow_Shop_Buy,				///> �̵���
	ItemFlow_Shop_Refresh,			///> �̵�ˢ��
	ItemFlow_Pay_Give,				///> ����
	ItemFlow_Lottory_Draw,			///> �齱
	ItemFlow_Checkin_Day,			///> ǩ��
	ItemFlow_Activity_Day,			///> ��Ծ��
	ItemFlow_Flower_Send,			///> �ͻ�����
	ItemFlow_Flower_Get,			///> �ͻ��õ�
	ItemFlow_Flower_GetRankReward,	///> �ͻ����н���
	ItemFlow_Achievement_Give,		///> �ɾͽ���
	ItemFlow_Fashion_Compose,		///> ʱװ�ϳ�
	ItemFlow_Fashion_SkillPoint,	///> ʱװ���ܵ�
	ItemFlow_Mail_Give,				///> �ʼ���
	ItemFlow_FindBack_Exp,			///> �����һ�
	ItemFlow_TShow_Vote,			///> T̨ͶƱ
	ItemFlow_Fatigue_Add,			///> ��������
	ItemFlow_Buy_Coin,				///> ������
	ItemFlow_Buy_Fatigue,			///> ��������
	ItemFlow_Buy_DragonCoin,		///> ��������
	ItemFlow_GM_Give,				///> GM��
	ItemFlow_GM_Clear,				///> GM����
	ItemFlow_Reward_System,			///> ϵͳ����
	ItemFlow_Reward_Online,			///> ���߽���
	ItemFlow_Reward_NextDay,		///> ���ս���
	ItemFlow_Reward_Days,			///> ��¼��������
	ItemFlow_Reward_Login,			///> ��¼����
	ItemFlow_Guild_Create,			///> ���ᴴ��
	ItemFlow_Guild_Card,			///> �����˿�
	ItemFlow_Guild_Checkin,			///> ����ǩ��
	ItemFlow_Guild_Fatigue,			///> ��������
	ItemFlow_Guild_Bonus,			///> ������
	ItemFlow_GaoJiGu_Fish,			///> ����
	ItemFlow_Pet_Feed,				///> ����ι��
	ItemFlow_Camp_Task,				///> ��Ӫ����
	ItemFlow_Auction_Take,			///> �����п�
	ItemFlow_Auction_Give,			///> �����и�
	ItemFlow_AchieveV2_Achieve,		///> �ɾͽ���
	ItemFlow_AchieveV2_AchievePoint,///> �ɾ͵�������
	ItemFlow_Stage_Pvp,				///> �����ӳ�
	ItemFlow_Chat_World,			///> ��������
	ItemFlow_IDIP_Oper,				///> IDIP
	ItemFlow_QA_AnswerRight,		///> �����Խ���
	ItemFlow_DRAGON_Enter,			///> ��������
	ItemFlow_Stage_Sweep,           ///> ɨ��
	ItemFlow_Tower_Sweep,			///> �ڰ����
	ItemFlow_Goddess_Reward,		///> Ů������ά��
	ItemFlow_Guild_Dare_Reward,		///> ������ս����
	ItemFlow_NestBuy,				///> ��Ѩ��������
	ItemFlow_AbysssBuy,				///> ��Ԩ��������
	ItemFlow_FriendGift,			///> ��������
	ItemFlow_EnhanceTrans,			///> ǿ��ת��
	ItemFlow_Expand_Seat,           ///> ��չ������  
	ItemFlow_BuyPay,				///> ��ֵ�����ʯ
	ItemFlow_BuyAileen,				///> ��ֵ������ÿ�����
	ItemFlow_BuyPayCard,			///> ��ֵ������/�¿�
	ItemFlow_BuyGrowthFund,			///> ��ֵ����ɳ�����
	ItemFlow_PayCardAward,			///> ��ȡ���¿�����
	ItemFlow_TitleLevelUp,			///> ͷ������
	ItemFlow_PayFirstAward,			///> ��ȡ�׳佱��
	ItemFlow_FirstPassRank,			///> ��ͨ
	ItemFlow_FirstPassCommend,		///> ��ͨ����
	ItemFlow_VipLevelGift,			///> ��ȡvip�ȼ����
	ItemFlow_GrowthFundAward,		///> ��ȡ�ɳ�������
	ItemFlow_ItemFindBackCost,		///> �����һ�
	ItemFlow_ItemFindBackGet,		///> �����һػ��
	ItemFlow_BuyIBItemByDiamond,	///> ������ʯ����IB����
	ItemFlow_BuyIBItemByDragonCoin,	///> ����������IB����
	ItemFlow_CreateAtlasTeam,	    ///> ͼ��ϵͳ�������
	ItemFlow_SpriteLevelUp,			///> ��������
	ItemFlow_SpriteEvolutionLevelUp,///> ��������
	ItemFlow_SpriteAwake,			///> �������
	ItemFlow_SpriteDecompose,		///> ����ֽ�
	ItemFlow_SpriteLottery,			///> ���鵰�齱
	ItemFlow_GaoJiGu_UseFish,		///> ʹ����
	ItemFlow_SuperRiskDice,		    ///> ��ð��
	ItemFlow_LevelSeal_Exchange,	///> ��ӡ���Ҷһ����
	ItemFlow_SuperRiskOpenBox,		///> ��ð�տ�����
	ItemFlow_SuperRiskBattle,		///> ��ð��ս��
	ItemFlow_SuperRiskAccelerate,	///> ��ð�ռ���
	ItemFlow_SuperRiskBuy,			///> ��ð�չ���
	ItemFlow_SuperRiskRebattle,		///> ��ð���ٴ���ս
	ItemFlow_StageDragonExp,        ///> ��֮Զ��
	ItemFlow_SmeltEquip,			///> װ��ϴ��
	ItemFlow_SmeltEmblem,			///> ����ϴ��
	ItemFlow_Tower_RefreshSweep,	///> �ڰ����ˢ��ɨ�����
	ItemFlow_Tower_FirstPass,		///> �ڰ������ͨ
	ItemFlow_BuyArenaCost,			///> ���򾺼�������
	ItemFlow_ClearArenaCD,			///> ���������CD
	ItemFlow_ResetProfession,		///> ����ְҵ
	ItemFlow_Buy_BlueBird,			///> ��������

	ItemFlow_UsePeck,				///> ʹ�����
	ItemFlow_CircleDraw,			///> Բ�̳齱
	ItemFlow_UsePetBorn,			///> ʹ�ó��ﵰ
	ItemFlow_UseSceneCountTicket,	///> ʹ���볡ȯ
	ItemFlow_OpenSystemGive,		///> ����ϵͳ�� 
	ItemFlow_SpActivity_GetReward,	///> ���ȡ������
	ItemFlow_SpActivity_BigPrize,	///> ���ȡ��
	ItemFlow_GuildCardMatch,		///> ���Ῠ�ƴ���
	ItemFlow_Auction_GetBack,       ///> ������ȡ��
	ItemFlow_LevelSealSelfGit,		///> ��ӡ����ѫ�½���

	ItemFlow_Garden_PlantSeed,		///> ��԰��ֲ
	ItemFlow_Garden_Fish,			///> ��԰����
	ItemFlow_Garden_Cooking,		///> ��԰���
	ItemFlow_Garden_Banquet,		///> ��԰

	ItemFlow_Guild_Wage,			///> ���Ṥ��

	ItemFlow_GuildAuct_Take,        ///> ��������

	ItemFlow_GoldClickGive,			///> ҡǮ��
	ItemFlow_GoldClickTake,			///> ҡǮ��

	ItemFlow_UsePandora,			///> ���˶���֮��

	ItemFlow_GuildAuct_Give,        ///> ��������

	ItemFlow_Donate,				///> �������
	
	ItemFlow_TeamCostTake,			///> �ͽ���
	ItemFlow_TeamCostGive,			///> �ͽ���
	ItemFlow_GuildInheritGive,		///> ���ᴫ��

	ItemFlow_RenameCost,			///> ��������
	ItemFlow_GuildBossAward,		///> ����boss����
	ItemFlow_GuildArenaAward,		///> ������̨ս����
	ItemFlow_GuildRankdAward,		///> ������������
	ItemFlow_GuildResWarAward,		///> �������ս����
	ItemFlow_GuildCampRankAward,	///> �����ð����������
	ItemFlow_SkyCityAward,			///> ��վ���������
	ItemFlow_GuildCardAward,		///> �����˿˽���
	ItemFlow_FlowerWeekRankAward,	///> �ʻ����н���
	ItemFlow_GuildLadderAward,		///> ��������������
	ItemFlow_QAAward,				///> �������н���
	ItemFlow_WorldBossAward,		///> ����boss����
	ItemFlow_PkWeekRankAward,		///> �����������н���
	ItemFlow_PlatFriendGift,		///> ƽ̨����gift
	ItemFlow_DragonExpRankAward,	///> ��֮Զ����������
	ItemFlow_DonateMemberItem,		///> �������
	ItemFlow_GuildAuctProfit,		///> ���������ֺ�
	ItemFlow_GuildAuctFailBack,		///> ���Ὰ��ʧ���˻�
	ItemFlow_TeamCostAward,			///> �ͽ���ÿ�ս���
	ItemFlow_QQVip,					///> ��Q��Ա
	ItemFlow_PayMember,				///> ��ֵ��Ա��Ȩ
	ItemFlow_TeamCostBack,			///> �ͽ��ŷ���
	ItemFlow_PetSkillBook,			///> ʹ�����＼����
	ItemFlow_Enchant_Take,			///> װ����ħ����
	ItemFlow_MakePartner,			///> ���ս�ѹ�ϵ 
	ItemFlow_WeekNest,              ///> ÿ�ܳ�Ѩ
	ItemFlow_PartnerChest,			///> ս�ѻ�Ծ�ȱ���
	ItemFlow_PokerTournamentAward,		///> �˿�֮������
	ItemFlow_PartnerShop,			///> ս���̵�
	ItemFlow_PokerTournamentCost,	///> �˿�֮����������
	ItemFlow_MentorComplete,		///> ʦͽ��ʦ
	ItemFlow_GuildTerrChall,		///> ���������ս
	ItemFlow_GuildTerrJoin,         ///> ������ز���
	
	ItemFlow_HeroBattle_Reward,		///> Ӣ��ս��ʤ��������ÿ�ղ��뽱��
	ItemFlow_HeroBattle_WeekReward,	///> Ӣ��ս��ÿ��ʤ������
	ItemFlow_HeroBattle_BuyHero,	///> Ӣ��ս����Ӣ��
	ItemFlow_SpActivity_Collect,	///> ����ռ������Ʒ
	ItemFlow_Desig,					///> �ɾͳƺ�
	ItemFlow_Forge_Take,			///> װ����������
	ItemFlow_Exchange_Dragoncoin,   ///> ��ʯ����ʱ���һ�ָ������������

	ItemFlow_ItemSell_Out,			///> ��Ʒ����
	ItemFlow_ItemSell_In,			///> ��Ʒ����
	ItemFlow_Fashion_TimeOut,		///> ʱװ����

	ItemFlow_ChangePro_Cost,		///> ��ְҵ
	ItemFlow_ChangePro_ItemTake,	///> ��ְҵ
	ItemFlow_ChangePro_ItemGive,	///> ��ְҵ

	ItemFlow_SpriteTrain,			///> ��������
	ItemFlow_SpriteResetTrain,		///> ������������

	ItemFlow_LeagueBattleRace,		///> ����������

	ItemFlow_SkillTwo,				///> ͬ����ȡ���ܵ�2
	ItemFlow_PayReward,             ///> ֧�����

	ItemFlow_MentorTask,	        ///> ʦͽ����
	ItemFlow_FlowerValentinesDayRankAward,	///> �ʻ����˽����н���

	ItemFlow_Compose_Common,		///> �ϳ���Ʒ����ͨ��

	ItemFlow_Team_UseTicket,        ///> �������Ʊ

	ItemFlow_Stage_ActivityOne,    ///> ��ؿ�1
	ItemFlow_Stage_ActivityTwo,    ///> ��ؿ�2
	ItemFlow_Stage_ActivityThree,  ///> ��ؿ�3

	ItemFlow_SpActivityPartner_GetReward,	///> ս���ջ��ȡ������
	ItemFlow_SpActivityBackFlow_GetReward,	///> �������ȡ������
	ItemFlow_SpActivityArgenta_GetReward,	///> ����ĸ�����ȡ������
	ItemFlow_SpActivityArgenta_GetDaily,	///> ����ĸ�����ȡÿ�ո���

	ItemFlow_WorldBoss_GuildAttr,		///> ����boss�������

	ItemFlow_HeroBattle_HeroTicket,		///> ʹ��Ӣ��ս������ȯ

	ItemFlow_HeroBattle_HeroTicket_GiveReward,	///> ��ӵ��Ӣ�ۣ�ʹ��Ӣ��ս������ȯ������
	ItemFlow_CombineServer_Reward,		///> �Ϸ�����
	ItemFlow_BreakAtlas	,				///> �ֽ�ͼ��
	ItemFlow_LoginActivity,				///��¼�

	ItemFlow_Flower520DayRankAward,	///> �ʻ�520���н���


	ItemFlow_FirstAchieveRank,		///> ������������
	ItemFlow_MilitaryRankAward,		///> ���������н���

	ItemFlow_IBItemCostMoney,		///> IBֱ���۷�
	ItemFlow_IBItemGiveItem,		///> IBֱ���͵���
	ItemFlow_ArenaStar_DianZan,     ///> ���������õ���
	ItemFlow_ABYSS_PARTY,			///> ��Ԩ�ɶ�
	ItemFlow_Military_Exchange,		///> ���α仯
	ItemFlow_GuildCampParty_RandomItem, ///> �����ɶԳ齱��Ʒ
	ItemFlow_GuildCampParty_ExchangeItem, ///> �����ɶԽ�����Ʒ
	ItemFlow_AbyssPartyTick,				  ///> ��Ԩ
	ItemFlow_ArtifactCompose_Take,	///> �����ϳ�����
	ItemFlow_ArtifactCompose_Give,	///> �����ϳɸ���
	ItemFlow_BuyExtraEmblemSlot,	///> �������¿�
	ItemFlow_IdipXinYueRPay = 231,		///> ���ô�R����,���ֵ���ܸģ������Ӱ�����ó�ֵ;
	ItemFlow_AbyssLoginMail,			///>��Ԩ�ɶԵ�¼����
	ItemFlow_UseItem_BagExpand,		///> ��������
	ItemFlow_ReturnSmeltStone_Equip,	///> װ��ϴ��ʯ����
	ItemFlow_ReturnSmeltStone_Emblem,	///> ����ϴ��ʯ����
	ItemFlow_Traja,						///> �������

	ItemFlow_TrajaAward,				///> ���㽱��

	ItemFlow_CustomBattle_Invalid, ///> ȫ��PK 
	ItemFlow_CustomBattle_Join, ///> ȫ��PK ����
	ItemFlow_CustomBattle_ClearCD, ///> ȫ��PK ����
	ItemFlow_CustomBattle_Create, ///> ȫ��PK ����
	ItemFlow_CustomBattle_RewardSystem, ///> ȫ��PK ����
	ItemFlow_CustomBattle_RewardCustom, ///> ȫ��PK ����
	ItemFlow_ItemExpirationTime_Take,	///> ��Ʒʱ�޹��ڿ۳�

	ItemFlow_IdipExchange, ///> idip��Ʒ�һ�;
	ItemFlow_PlatShare_Award,			///> ƽ̨������

	ItemFlow_FlowerActivityDayRankAward,	///> �ʻ�����н���

	ItemFlow_FASHION_ACTIVATE_COLOR,	///> ʱװ�����ͻ���ɫ
	ItemFlow_Team_DragonHelp,          ///> ������ս

    ItemFlow_WeekEnd4v4_MONSTERFIGHT,               ///��ĩ�ɶԹ�����Ҷ�
    ItemFlow_WeekEnd4v4_GHOSTACTION,               ///��ĩ�ɶ������ж�
    ItemFlow_WeekEnd4v4_LIVECHALLENGE,               ///��ĩ�ɶ��������ս
    ItemFlow_WeekEnd4v4_CRAZYBOMB,               ///��ĩ�ɶԷ��ը����
    ItemFlow_WeekEnd4v4_HORSERACING,               ///��ĩ�ɶ��Ŷ�����
	ItemFlow_BackFlowInit_Equip,		///> �ع������װ������
	ItemFlow_BackFlowInit_Emblem,		///> �ع���������¸���
	ItemFlow_BackFlowInit_Jade,			///> �ع������������

	ItemFlow_MayhemPointAward, //���Ҷ����ֽ���
	ItemFlow_MayhemRankAward, //���Ҷ����н���
	ItemFlow_MayhemJoinAward, //���Ҷ����ֽ���

	ItemFlow_ArtifactComposeMulti_Take,		///> ����һ���ϳ�����
	ItemFlow_ArtifactComposeMulti_Give,		///> ����һ���ϳɸ���
	ItemFlow_MobaBattle_Reward,				///> Ӣ��Ͽ��ÿ��ʤ������
	ItemFlow_MobaBattle_WeekReward,			///> Ӣ��Ͽ��ÿ��ʤ������
	ItemFlow_Sprite_BuySpriteEgg_Take,		///> ������ҩˮ�;��鵰����
	ItemFlow_Sprite_BuySpriteEgg_Give,		///> ������ҩˮ�;��鵰����

	ItemFlow_DragonExp_TongGuan,            ///> ��֮Զ��ͨ��
	ItemFlow_Personal_Honour_Award,			///> ����ְҵ������ҫ����
	ItemFlow_AncientTimes,					///> �ϹŻ����
	ItemFlow_DuckTickets,					///> ��Ѽ��Ʊ
	ItemFlow_DuckAward,						///> ��Ѽ��������
	ItemFlow_DuckDayTickets,			///>��Ѽÿ����Ʊ

	ItemFlow_Marriage_Marry,		///> ���
	ItemFlow_Marriage_Divorce,		///> ���

	ItemFlow_BioHell,						///> ��������Ȯ���㽱��;
	ItemFlow_WeddingFlow,					///> �������
	ItemFlow_WeddingFireworks,				///> �����̻�
	ItemFlow_WeddingCandy,					///> ����ϲ��
	ItemFlow_MarriageLiveness,				///> ����Ծ��
	ItemFlow_WeddingThanks,					///> �����л

    ItemFlow_WeekEnd4v4_DUCK,               ///> ��ĩ�ɶԹ�Ѽ��
	ItemFlow_WorldBossKillAward,			///> ����boss��ɱ���뽱��

	ItemFlow_Activity_Coin,					///> ��Ծ�ȣ��ع��Ծ��
	ItemFlow_ArtifactRecastCost,			///> ��������
	ItemFlow_ArtifactFuse,					///> �����ں�
	ItemFlow_ArtifactFuseUseStone,			///> �����ں�ʹ������ʯ
	ItemFLow_ArtifactInscription,			///> ����ʹ�ü�������

	ItemFlow_BackFlowBuy_Take,				///> ����ֱ����
	ItemFlow_BackFlowBuy_Give,				///> ����ֱ����
	ItemFlow_SpriteRebirth,					///> ��������

	ItemFlow_TransOhter,					///> �����

	ItemFlow_SpActivity_WeekNest_GetReward,	///> ��ĩ�ǳ�Ѩ���ȡ������

	ItemFlow_Equip_Upgrade,					///> װ������

	ItemFlow_DragonGuildCreate,				///> С�ֶӴ���
	ItemFlow_DragonGuildTaskChest,          ///> С�ֶ���ȡ������
	ItemFlow_DragonGuildShop,				///> С�ֶ��̵�
	ItemFlow_PartnerCompenstate,			///> ս��ϵͳ����
    ItemFlow_GoalAwards_GetReward,			///> Ŀ�꽱������ȡ����
	ItemFlow_WeeklyTaskRefresh,				///> ������ˢ��
	ItemFlow_BattleFieldWeekPoint,			///> ս���ܻ���
	ItemFlow_Equip_FuseAddExp,				///> װ�������ӽ���
	ItemFlow_Equip_FuseBreak,				///> װ������ͻ��
	ItemFlow_NpcFeeling_GiveNpc,            ///> NPC�øж� ���͸�npc
	ItemFlow_TaskTurnOver,					///> �����Ͻ���Ʒ
	ItemFlow_Equip_FuseCompensation,		///> װ����������Ҳ���
    ItemFlow_CompeteDragonThrough,			///> ��������
	ItemFlow_NpcFeeling_EX_Role2Npc,		///> NPC�øж� ���� ��Ҹ�NPC
	ItemFlow_NpcFeeling_EX_Npc2Role,        ///> NPC�øж� ���� NPC�����
	ItemFlow_NpcFeeling_NpcReturn,          ///> NPC�øж� NPC����
	ItemFlow_NpcFeeling_NpcBuyCount,        ///> NPC�øж� �������
	ItemFlow_BuyDailyTaskRefreshCount,		///> �����ճ�����ˢ�´���

	ItemFlow_Survive_RankRward,             ///> ����������������
	ItemFlow_Survive_DayReward,             ///> ���������ճ�����
	ItemFlow_ConsumeRebate,					///>�������ѷ���

	ItemFlow_Stage_PeerTake,				///> �Ŵ�
	ItemFlow_Stage_BoxGive,					///> �鱦��
	ItemFlow_ActivatePre,					///> ���ѳ�̬��Ȩ����;
	ItemFlow_GuildTerrFeats,				///> ������ع�ѫ�ʼ�����

	ItemFlow_KingBackGive,                  ///> �����߹�����ӽ���

	ItemFlow_Material,						///> ����
	ItemFlow_BackFlow_GetTreasure,			///> �������ȡ����
	ItemFlow_BackFlowShop_Diamond,			///> ����������̵�����ʯ
	ItemFlow_BackFlowShop_DragonCoin,		///> ����������̵�������	
	ItemFlow_BackFlowShop_Give,				///> ����������̵깺�����Ʒ
	ItemFlow_GuildBonus,					///> �������
	ItemFlow_BackFlowChargeBack,			///> ������ʷ��ֵ����
	ItemFlow_Equip_ForgeCompensation,		///> װ����������Ҳ���
	ItemFlow_AncientTimesTake,				///> �ϹŻ�۳��ϴλ���
////> ע�⣺�� �¼�����ֻ���������¼ӣ����ܴ������������id  ���������ע�ͣ�
};


//(����)��������һ��ԭ��
enum EPlayerExpFlowType
{
};

//(����)������������ԭ��
enum EPlayerExpFlowSubType
{
};

//(����)��Ӫ�����
enum EActivityType
{
};


//�齱���ͣ���������(����)


//����״̬
enum ETaskState
{
	TASK_STATE_ACCEPTED			= 0,		//����
	TASK_STATE_COMPLETED		= 1,		//���
};

#define MAX_PAY_BILL_COUNT 100 //���涩����;
#endif
