#ifndef _H_IDIP_H__
#define _H_IDIP_H__

#include <iomanip>

enum EIdipServer
{
	IDIP_SERVER_NONE = 0,//MS直接处理返回
	IDIP_SERVER_MS = 1,//MS读取数据处理
	IDIP_SERVER_GS = 2,//game server
	IDIP_SERVER_LOGIN = 3,// login server
	IDIP_SERVER_OTHER = 4,//other
};
enum EIdipNoticeType
{
	NOTICE_TYPE_BEFORE_GAME = 1,	//游戏前公告
	NOTICE_TYPE_AUTHORIZE = 2,		//授权后公告
	NOTICE_TYPE_ENTER_GAME  = 3,    //进入游戏后公告(文字)
	NOTICE_TYPE_ENTER_GAME_PIC = 4, //进入游戏后公告(图片)
	NOTICE_TYPE_ENTER_FACE = 5,		//拍脸图
	NOTICE_TYPE_ENTER_GAME_TITLEPIC = 6, //进入游戏后公告（图片标题）
};

#define MAX_NOTICECONTENT_LEN 2500 //公告的长度
#define MAX_BANINFOLIST_NUM 50
#define MAX_GUILDMEMBERLIST_NUM 50 //工会成员列表的数组长度
#define FOREVER_TIME 10 * 365 * 86400 //永久默认10年
#define MAX_NOTICELIST_NUM 3
#define MAX_PARTITIONLIST_NUM 100
#define MAX_BAGLIST_NUM 200 //背包道具列表的数组长度
#define MAX_DRAGONJADENUM 4 //龙玉个数
#define MAX_BADGEATTRNUM 3 //纹章洗练属性个数;
#define MAX_REFINEATTRNUM 1 //锻造属性;
#define MAX_SMELTATTRNUM 2 //洗练属性;
#define MAX_MAILLIST_NUM 15 //邮件信息列表的数组长度
#define MAX_FAIRYLIST_NUM 30 //精灵列表的数组长度
#define MAX_EQUIPITEMLIST_NUM 50 //装备列表的数组长度


#define GODDNESS_TEARS_ID 88 //女神的眼泪ID
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
	GAME_NOTICE_TYPE_SYSTEM = 0, //系统;
	GAME_NOTICE_TYPE_WORLD = 1,  //世界;
	GAME_NOTICE_TYPE_LAMP = 2,	 //走马灯;
	GAME_NOTICE_TYPE_ALL = 99,   //全部
};

enum EIdipBanRankType
{
	BanRankTypeWorldBoss = 1,			//世界boss
	BanRankTypeRoleGuildBoss = 2,		//公会boss
	BanRankTypeALL = 99,
};

enum EIdipBanPlayType
{
	BanPlayTypeDailyPlay = 1,			//日常玩法
	BanPlayTypeMutilActivity = 2,		//所有玩法
	BanPlayTypeALL = 99,				//全选
};

enum EIdipBanOfflineRankType
{
	BanOfflineRankType_Pk = 1,			//天梯
	BanOfflineRankType_Arena = 2,		//竞技场
	BanOfflineRankType_Tower = 3,		//黑暗神殿
	BanOfflineRankType_Flower = 4,		//魅力
	BanOfflineRankType_Guild = 5,		//公会
	BanOfflineRankType_GuildBoss = 6,	//公会boss
	BanOfflineRankType_ALL = 99,		//all
};

enum EIdipDoDelItemType
{
	DoDelItemType_Fashion = 1,	//时装
	DoDelItemType_Item = 2,		//装备
	DoDelItemType_Emblem = 3,	//纹章
	DoDelItemType_Jade = 4,		//装备上的龙玉
};

enum EIdipCmd
{
	IDIP_DO_UPDATE_GOLD_REQ = 4137,							//修改金币（游戏币）
	IDIP_DO_UPDATE_GOLD_RSP = 4138,							//

	IDIP_DO_UPDATE_EXP_REQ  = 4139,							//修改经验值
	IDIP_DO_UPDATE_EXP_RSP	= 4140,

	IDIP_DO_UPDATE_PHYSICAL_REQ = 4141,						//修改体力值
	IDIP_DO_UPDATE_PHYSICAL_RSP = 4142,

	IDIP_DO_SEND_ITEM_REQ = 4143,							//赠送道具
	IDIP_DO_SEND_ITEM_RSP = 4144,

	IDIP_DO_DEL_ITEM_REQ = 4145,							//扣除道具
	IDIP_DO_DEL_ITEM_RSP = 4146,

	IDIP_DO_ACTIVE_USR_REQ = 4147,							//激活帐号
	IDIP_DO_ACTIVE_USR_RSP = 4148,

	IDIP_DO_BAN_USR_REQ = 4149,								//封号
	IDIP_DO_BAN_USR_RSP = 4150,

	IDIP_DO_UNBAN_USR_REQ = 4151,						    //解除封号
	IDIP_DO_UNBAN_USR_RSP = 4152,

	IDIP_QUERY_USER_INFO_REQ = 4153,						//当前个人信息查询
	IDIP_QUERY_USER_INFO_RSP = 4154,

	IDIP_QUERY_REVERSE_USER_INFO_REQ = 4155,				//当前个人信息逆查询
	IDIP_QUERY_REVERSE_USER_INFO_RSP = 4156,

	IDIP_QUERY_ROLE_INFO_REQ = 4157,						//当前角色信息查询
	IDIP_QUERY_ROLE_INFO_RSP = 4158,

	IDIP_QUERY_PARTITION_INFO_REQ = 4159,					//小区查询
	IDIP_QUERY_PARTITION_INFO_RSP = 4160,

	IDIP_QUERY_BAN_INFO_REQ = 4161,							//查询全区封号状态请求
	IDIP_QUERY_BAN_INFO_RSP = 4162,

	IDIP_DO_UPDATE_DIAMOND_REQ = 4163,						//修改钻石（代币）
	IDIP_DO_UPDATE_DIAMOND_RSP = 4164,						//
	 
	IDIP_DO_UPDATE_DRAGON_GOLD_REQ = 4165,					//修改龙币 
	IDIP_DO_UPDATE_DRAGON_GOLD_RSP = 4166,

	IDIP_DO_UPDATE_HONOUR_REQ = 4167,						//修改荣誉值
	IDIP_DO_UPDATE_HONOUR_RSP = 4168,

	IDIP_DO_MASK_CHAT_REQ = 4169,							//禁言请求
	IDIP_DO_MASK_CHAT_RSP = 4170,

	IDIP_QUERY_BAG_REQ = 4171,								//当前背包查询
	IDIP_QUERY_BAG_RSP = 4172,

	IDIP_QUERY_BADGE_REQ = 4173,							//当前纹章查询
	IDIP_QUERY_BADGE_RSP = 4174,

	IDIP_QUERY_RUNNING_NOTICE_REQ = 4175,					//查询公告
	IDIP_QUERY_RUNNING_NOTICE_RSP = 4176,					

	IDIP_QUERY_EQUIPMENT_REQ = 4177,						//当前装备查询
	IDIP_QUERY_EQUIPMENT_RSP = 4178,

	IDIP_DO_SEND_NOTICE_REQ = 4179,							//发送公告
	IDIP_DO_SEND_NOTICE_RSP = 4180,
	
	IDIP_DO_SEND_ROLLING_MSG_REQ = 4181,					//发送走马灯公告
	IDIP_DO_SEND_ROLLING_MSG_RSP = 4182,

	IDIP_QUERY_RUNNING_ROLLING_MSG_REQ = 4183,				//查询走马灯公告
	IDIP_QUERY_RUNNING_ROLLING_MSG_RSP = 4184,

	IDIP_DO_STOP_ROLLING_MSG_REQ = 4185,					//删除走马灯公告
	IDIP_DO_STOP_ROLLING_MSG_RSP = 4186,

	IDIP_QUERY_PARTY_INFO_REQ = 4187,						//查询工会信息
	IDIP_QUERY_PARTY_INFO_RSP = 4188,

	IDIP_DO_GM_UPDATE_DIAMOND_REQ = 4189,					//修改钻石（代币）管理端请求
	IDIP_DO_GM_UPDATE_DIAMOND_RSP = 4190,

	IDIP_DO_GM_UPDATE_DRAGON_GOLD_REQ =	4191,				//修改龙币（代币）管理端请求
	IDIP_DO_GM_UPDATE_DRAGON_GOLD_RSQ = 4192,

	IDIP_DO_GM_SEND_ITEM_REQ = 4193,						//赠送道具
	IDIP_DO_GM_SEND_ITEM_RSP = 4194,

	IDIP_DO_GM_UPDATE_PHYSICAL_REQ = 4195,					//修改体力值（管理端）请求
	IDIP_DO_GM_UPDATE_PHYSICAL_RSP = 4196,

	IDIP_AQ_QUERY_OPENID_INFO_REQ = 4197,					//查询openid基本信息（AQ）请求
	IDIP_AQ_QUERY_OPENID_INFO_RSP = 4198,

	IDIP_AQ_QUERY_USER_INFO_REQ = 4199,						//查询账号下角色信息（AQ）请求
	IDIP_AQ_QUERY_USER_INFO_RSP = 4200,

	IDIP_AQ_DO_SEND_MSG_REQ = 4201,							//发消息（AQ）请求
	IDIP_AQ_DO_SEND_MSG_RSP = 4202,

	IDIP_AQ_DO_UPDATE_GOLD_COIN_REQ = 4203,					//修改游戏币数量（AQ）请求
	IDIP_AQ_DO_UPDATE_GOLD_COIN_RSP = 4204,

	IDIP_AQ_DO_UPDATE_DIAMOND_REQ = 4205,					//修改钻石数量（AQ）请求
	IDIP_AQ_DO_UPDATE_DIAMOND_RSP = 4206,

	IDIP_AQ_DO_CLEAR_SCORE_REQ = 4207,						//清零游戏分数（AQ）请求
	IDIP_AQ_DO_CLEAR_SCORE_RSP = 4208,

	IDIP_AQ_DO_SET_SCORE_REQ = 4209,						//设置游戏分数（AQ）请求
	IDIP_AQ_DO_SET_SCORE_RSP = 4210,

	IDIP_AQ_QUERY_BOSS_BATTLE_INFO_REQ = 4211,				//查询Boss战当前信息（AQ）请求
	IDIP_AQ_QUERY_BOSS_BATTLE_INFO_RSP = 4212,

	IDIP_AQ_DO_BAN_RANKING_REQ = 4213,						//禁止实时排行榜
	IDIP_AQ_DO_BAN_RANKING_RSP = 4214,

	IDIP_AQ_QUERY_PARTY_NOTICE_REQ = 4215,				    //查询公会公告（AQ）应答
	IDIP_AQ_QUERY_PARTY_NOTICE_RSP = 4216,

	IDIP_AQ_DO_SET_NOTICE_REQ = 4217,						//设置用户签名和公会公告（AQ）请求
	IDIP_AQ_DO_SET_NOTICE_RSP = 4218,

	IDIP_AQ_DO_INITIAL_ACCOUNT_REQ = 4219,					//初始化帐号
	IDIP_AQ_DO_INITIAL_ACCOUNT_RSP = 4220,

	IDIP_AQ_DO_ZEROPROFIT_REQ = 4221,						//零收益
	IDIP_AQ_DO_ZEROPROFIT_RSP = 4222,

	IDIP_AQ_DO_BAN_PLAY_REQ = 4223,							//禁止指定玩法（AQ）请求
	IDIP_AQ_DO_BAN_PLAY_RSP = 4224,

	IDIP_AQ_DO_BAN_ALL_PLAY_REQ = 4225,						//禁止所有玩法接口（AQ）请求
	IDIP_AQ_DO_BAN_ALL_PLAY_RSP = 4226,

	IDIP_AQ_DO_BAN_USR_REQ = 4227,							//封号（AQ）请求
	IDIP_AQ_DO_BAN_USR_RSP = 4288,


	IDIP_AQ_DO_MASK_CHAT_REQ = 4229,						//禁言（AQ）请求
	IDIP_AQ_DO_MASK_CHAT_RSP = 4230,
	
	IDIP_AQ_DO_CLEAR_SPEAK_REQ = 4231,						//发言清除（AQ）请求
	IDIP_AQ_DO_CLEAR_SPEAK_RSP = 4232,


	IDIP_AQ_DO_SEND_NOTICE_REQ = 4233,						//发送公告（AQ）请求
	IDIP_AQ_DO_SEND_NOTICE_RSP = 4234,

	IDIP_AQ_DO_RELIEVE_PUNISH_REQ = 4235,					//解除处罚（AQ）请求
	IDIP_AQ_DO_RELIEVE_PUNISH_RSP = 4236,

	IDIP_AQ_DO_BAN_RANKING_OFFLINE_REQ = 4237,				//禁止参与排行榜接口（离线榜）（AQ）请求
	IDIP_AQ_DO_BAN_RANKING_OFFLINE_RSP = 4238,		

	IDIP_DO_GM_UPDATE_GOLD_REQ = 4239,						//修改金币（代币）管理端请求
	IDIP_DO_GM_UPDATE_GOLD_RSP = 4240,

	IDIP_QUERY_INDIVIDUAL_BAN_INFO_REQ = 4241,				//查询个人封号信息请求
	IDIP_QUERY_INDIVIDUAL_BAN_INFO_RSP = 4242,

	IDIP_QUERY_GUILD_INFO_REQ = 4243,						//查询工会信息请求
	IDIP_QUERY_GUILD_INFO_RSP = 4244,

	IDIP_QUERY_GUILD_MEMBER_INFO_REQ = 4245,				//查询工会成员信息请求
	IDIP_QUERY_GUILD_MEMBER_INFO_RSP = 4246,

	IDIP_QUERY_FAIRY_INFO_REQ = 4247,						//当前精灵信息查询请求
	IDIP_QUERY_FAIRY_INFO_RSP = 4248,

	IDIP_QUERY_HORSE_INFO_REQ = 4249,						//当前坐骑信息查询请求
	IDIP_QUERY_HORSE_INFO_RSP = 4250,

	IDIP_DO_UNMASK_CHAT_REQ = 4251,							//解除禁言请求
	IDIP_DO_UNMASK_CHAT_RSP = 4252,

	IDIP_DO_SEND_MAIL_REQ = 4253,							//全服邮件请求
	IDIP_DO_SEND_MAIL_RSP = 4254,

	IDIP_DO_CONTROL_FUNCTION_REQ = 4259,					//功能开关请求
	IDIP_DO_CONTROL_FUNCTION_RSP = 4260,

	IDIP_DO_NAME_ACT_REQ = 4261,							//称号激活请求
	IDIP_DO_NAME_ACT_RSP = 4262,

	IDIP_QUERY_FASHION_REQ = 4263,							//当前时装查询请求
	IDIP_QUERY_FASHION_RSP = 4264,

	IDIP_QUERY_GUILD_INFO_BACK_REQ = 4265,					//工会信息逆查询请求
	IDIP_QUERY_GUILD_INFO_BACK_RSP = 4266,

	IDIP_DO_DEL_ITEM_OUTBAG_REQ = 4267,						//删除背包外道具（时装，纹章，装备）请求
	IDIP_DO_DEL_ITEM_OUTBAG_RSP = 4268,

	IDIP_DO_SEND_PERSON_MAIL_REQ = 4269,					//个人邮件请求
	IDIP_DO_SEND_PERSON_MAIL_RSP = 4270,

	IDIP_QUERY_USER_INFO_WEEK_REQ = 4277,					//本周个人信息查询请求
	IDIP_QUERY_USER_INFO_WEEK_RSP = 4278,

	IDIP_DO_SET_NO_PLAY_REQ = 4279,							//成长守护（禁玩与解禁）请求
	IDIP_DO_SET_NO_PLAY_RSP = 4280,

	IDIP_QUERY_GUILD_TASK_REQ = 4281,						//当前公会任务查询请求;
	IDIP_QUERY_GUILD_TASK_RSP = 4282,

	IDIP_DO_XINYUE_RED_REQ = 4283,							//心悦红点激活请求
	IDIP_DO_XINYUE_RED_RSP = 4284,

	IDIP_DO_UPDATE_NAME_REQ = 4285,							//玩家名字修改请求
	IDIP_DO_UPDATE_NAME_RSP = 4286,

	IDIP_DO_UPDATE_GUILD_NAME_REQ = 4287,					//公会名字修改请求
	IDIP_DO_UPDATE_GUILD_NAME_RSP = 4288,

	IDIP_DO_XINYUE_R_RECHARGE_REQ = 4289,					//修改钻石（代币）（大R代充）请求
	IDIP_DO_XINYUE_R_RECHARGE_RSP = 4290,

	IDIP_DO_CHANGE_REQ = 4291,								//道具兑换请求
	IDIP_DO_CHANGE_RSP = 4292,

	IDIP_DO_UPDATE_PRESTIGE_REQ = 4293,						//修改公会威望请求
	IDIP_DO_UPDATE_PRESTIGE_RSP = 4294,

	IDIP_DO_UPDATE_GUILD_EXP_REQ = 4295,					//修改公会经验请求
	IDIP_DO_UPDATE_GUILD_EXP_RSP = 4296,

	IDIP_QUERY_MAIL_STATUS_REQ = 4297,						//当前玩家邮件状态查询请求
	IDIP_QUERY_MAIL_STATUS_RSP = 4298,

	IDIP_DO_UPDATE_BACK_ATTR_REQ = 4299,					//修改玩家回归服属性请求
	IDIP_DO_UPDATE_BACK_ATTR_RSP = 4300,

	IDIP_QUERY_BACK_ATTR_REQ = 4301,						//查询玩家回归服属性请求
	IDIP_QUERY_BACK_ATTR_RSP = 4302,

	IDIP_QUERY_ITEM_ID_TO_NAME_REQ = 4303,					//道具ID与道具名互查请求
	IDIP_QUERY_ITEM_ID_TO_NAME_RSP = 4304,

	IDIP_QUERY_FIGHT_PUNISH_REQ = 4305,						//查询英雄战场处罚情况请求
	IDIP_QUERY_FIGHT_PUNISH_RSP = 4306,

	IDIP_DO_CLEAR_FIGHT_PUNISH_REQ = 4307,					//解除英雄战场处罚请求
	IDIP_DO_CLEAR_FIGHT_PUNISH_RSP = 4308,

	IDIP_DO_XIAOYUE_RED_REQ = 4309,							//小悦红点激活请求
	IDIP_DO_XIAOYUE_RED_RSP = 4310,

	IDIP_DO_ACTIVE_ACHIEVEMENT_REQ = 4311,					//成就激活请求
	IDIP_DO_ACTIVE_ACHIEVEMENT_RSP = 4312,

	IDIP_DO_ADD_DEL_WHITE_LIST_REQ = 4313,					//添加与删除白名单请求
	IDIP_DO_ADD_DEL_WHITE_LIST_RSP = 4314,

	IDIP_QUERY_1V1_RANK_REQ = 4317,							//查询区服1v1排行请求
	IDIP_QUERY_1V1_RANK_RSP = 4318,

	IDIP_QUERY_WHITE_LIST_REQ = 4319,						//查询白名单情况请求
	IDIP_QUERY_WHITE_LIST_RSP = 4320,

	IDIP_DO_COIN_CONSUME_TO_GAME_REQ = 4321,				//代币消耗上报游戏内请求
	IDIP_DO_COIN_CONSUME_TO_GAME_RSP = 4322,

	IDIP_QUERY_ROLE_CLOTHES_REQ = 4323,						//角色时装合成情况查询请求
	IDIP_QUERY_ROLE_CLOTHES_RSP = 4324,
};


enum EIdipErrCode
{
	IDIP_ERR_SUCCESS							= 0,		//成功
	IDIP_ERR_PLAYER_NOT_EXIST					= 1,		//账号或角色不存在
	IDIP_ERR_ROLE_OFFLINE						= 2,		//角色不在线
	IDIP_ERR_INVALID_PARA						= 3,		//无效的请求参数
	IDIP_ERR_ALREADY_ACTIVE						= 4,		//已被激活过
	IDIP_ERR_SEND_MAIL							= 5,		//发送邮件失败
	IDIP_ERR_INVALID_CMDID						= 6,		//没有该协议ID

	IDIP_ERR_UNKNOWN							= 10,		//其他错误
	IDIP_ERR_INVALID_REQ_NUM					= 11,		//请求数量非法
	IDIP_ERR_INVALID_ITEM_ID					= 20,		//道具ID非法
	IDIP_ERR_INVALID_OPEN_ID					= 21,		//OpenID非法
	IDIP_ERR_INVALID_TIME						= 22,		//时间格式非法
	IDIP_ERR_INVALID_LAMPID						= 23,		//不存在有效的跑马灯公告
	IDIP_ERR_NONE_LAMPNOTICE					= 24,		//没有进行的跑马灯公告
	IDIP_ERR_INVALID_GUILDID					= 25,		//无效的工会ID
	IDIP_ERR_INVALID_SERVERID					= 26,		//无效的服务器ID
	IDIP_ERR_MAX_UPDATE_NUM						= 27,		//超出最大修改上限;
	IDIP_ERR_NOTJOIN_GUILD						= 28,		//没有加入公会;
	IDIP_ERR_TIMEOUT							= 29,		//超时
	IDIP_ERR_NUM_NOT_ENOUGH						= 30,		//数量不足
	IDIP_ERR_INVALID_ORDERID					= 31,		//无效的流水号
	IDIP_ERR_KICKOUT							= 32,		//踢下线失败
	IDIP_ERR_NOLOGIN							= 33,		//idipserver与login不通;
	IDIP_ERR_NOMS								= 34,		//idipserver与游戏服务器不通;
	IDIP_ERR_SAME_LAMPID						= 35,		//已存在该id的跑马灯
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