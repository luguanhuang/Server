#ifndef __RESWARMGRDEF_H__
#define __RESWARMGRDEF_H__

///////////////////////////////////////////////////////////////////////////工会buff/////////////////////////////////////////////////////////////////////////////
#include "notice/notice.h"
#include "role/rolemanager.h"

namespace GuildResWar
{
	#define MAX_GUILD_WAR_GROUP 9
	#define GUILD_MAX_TEAM 3
	#define GUILDRESWAR_TIME_SPAN	1
	enum GuildResType
	{
		GuildResPVE =1,
		GuildResPVP =2,
	};

	enum GuildCountDownType
	{
		eResWarReady    = 1,		//准备倒计时
		eResWarEnd		= 2,		//结束倒计时
	};

	enum GuildResFlow
	{
		eGuildResFlowIdle				= 0,		//空闲状态
		eGuildResFlowGroup      		= 1,		//分组
		eGuildResFlowExplore			= 2,		//探索
		eGuildResFlowAward				= 3,		//派奖
		eGuildResFlowEnd				= 4,		//结束流程
	};
	enum GuildResWarState
	{
		eGuildResStateIdle			  = 0,			//
		eGuildResStateExploreIng	  = 1,			//探索cd中
		eGuildResStateExploreEnd	  = 2,			//探索出结果
		eGuildResStateMatch			  = 3,
		eGuildResStateFight			  = 4,			//战斗中
		eGuildResStateResult		  = 5,			//战斗出结果
		eGuildResStateCancel		  = 6,			//取消
	};

	enum ResWarRefresh
	{
		eResWarRank                   = 1,
		eResWarCD					  = 2,
		eResWarUseItem				  = 3,
		eResWarCardCD				  = 4,
		eResWarBuffItem				  = 5,
		eResWarMineID				  = 6,
	};

	enum ResWarMsgType
	{
		eResWarMsgTypeIcon           = 1,
		eResWarMsgTypeNotice         = 2,
		eResWarMsgTypeTime			 = 3,
		eResWarMsgTypeUseCard		 = 4,
		eResWarMsgTypeUseCardEnd	 = 5,
		eResWarOpen5Min				 = 6,
		eResWarFinal5Min			 = 7,
		eResWarAwardTypeIcon         = 8,
	};

	struct GuildFightTeam;

	struct GuildFightRole
	{
		UINT64 nRole;
		UINT32 nJob;
		UINT32 nLevel;
		UINT32 nFight;
		std::string szName;
		bool bLost;
		UINT32 nRes;
		bool bIcon;
		GuildFightTeam *pGuildFightTeam;
		GuildFightRole():nRole(0),nJob(0),nLevel(0),nFight(0),szName(""),bLost(false),nRes(0),bIcon(true),pGuildFightTeam(NULL){}
	};


	struct GuildTeam;
	struct GuildFightTeam
	{
		std::map<UINT64, GuildFightRole*> mapFightRole;
		UINT32 nKiller;	//击杀人数
		UINT64 nHurt;	//击杀伤害
		UINT32 nWin;	//胜利场数
		UINT64 nFight;	//公会战斗力
		UINT32 nState;	//状态
		UINT32 nTime;	//time 探索结束时间
		UINT32 nUniID;	//分组ID
		UINT64 nGuildID;//工会ID
		UINT32 nTeamID;	//队伍ID
		std::vector<UINT32> vecMineral;
		std::vector<UINT32> vecBuff;
		GuildTeam *pGuildTeam;
		GuildFightTeam():nKiller(0),nHurt(0),nWin(0),nFight(0),nState(0),nTime(0),nUniID(0),nGuildID(0),nTeamID(0),pGuildTeam(NULL){}
	};

	struct GroupGuildFightTeam
	{
		UINT32 nUniID;
		GuildFightTeam *pSelf;
		GuildFightTeam *pTarget;
		UINT32 nFightTime;
		GroupGuildFightTeam():nUniID(0),pSelf(NULL),pTarget(NULL),nFightTime(0){}
	};

	struct SimpleGuildInfo
	{
		std::string szName;
		UINT32 nPrestige;
		UINT32 nIcon;
		SimpleGuildInfo():szName(""),nPrestige(0),nIcon(0){}
	};



	struct  GroupGuildInfo;
	struct  GuildTeam
	{
		UINT64 nGuildID;							//工会ID
		std::string szName;							//工会名称
		UINT32 nIcon;								// 图标
		UINT32 nResValue;							//资源值
		UINT32 nCD;									//探索时间
		UINT32 nNextUseBuffTime;					//下一次使用Buff时间
		UINT32 nFight;								
		UINT32 nTime;								
		UINT32 nState;								//默认是正常，被debuff后是？
		UINT32 nAddition;							//加成 默认是0
		UINT32 nPrestige;
		GroupGuildInfo *pGroupGuildInfo;
		std::map<UINT32, UINT32> mapBuff;			//工会仓库
		std::map<UINT32, GuildFightTeam *> mapTeam;
		std::vector<Notice*> vecNotice;
		GuildTeam():nGuildID(0),szName(""),nIcon(0),nResValue(0),nCD(0),nNextUseBuffTime(0),nFight(0),nTime(0),nState(0),nAddition(0),nPrestige(0),pGroupGuildInfo(NULL){}
	};

	struct GroupGuildInfo
	{
		UINT32 nGroupID;
		std::map<UINT64, GuildTeam*> mapGroupGuild;
		GroupGuildInfo():nGroupID(0){}
	};

	enum GuildBuffState
	{
		eStealGuildRes=1,   //1偷取目标公会资源总量5%~10%
		eAddEnemyTime =2,   //2目标公会探索时间延长X秒时间
		eSubSelfTime  =3,   //3己方公会探索时间缩短X秒时间
		eAddSelfRes   =4,   //4己方公会成员探索获得的资源量增加X倍时间
		eEnemyUnKnown =5,   //5敌方公会所有成员探索结果变为“？”时间
		eClearDeBuff  =6,   //6清除所有减益仪式卡效果
	};

	enum GuildBuffCamp
	{
		eGuildBuffCampSelf  = 1,
		eGuildBuffCampEnemy = 2,
	};

	struct GuildBuffInfo
	{
		UINT64 nGuildID;
		UINT32 nID;
		time_t nTime;
		bool bDel;								//是否删除
		GuildBuffInfo():nGuildID(0),nID(0),nTime(0),bDel(false){}
	};

};

#endif
