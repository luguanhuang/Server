#ifndef __RESWARMGR_H__
#define __RESWARMGR_H__
#include "reswarmgrdef.h"
#include "timer.h"
#include "table/GuildMineralBattleReward.h"



using namespace GuildResWar;
class CRole;
class Team;
class CSceneSwitch;
class Guild;

//排行榜的相关信息
struct  GuildWarRank
{
	UINT64 nGuildID;
	std::string szName;
	UINT32 nResValue;
	UINT32 nPrestige;
	UINT32 nIcon;
	GuildWarRank():nGuildID(0),szName(""),nResValue(0),nPrestige(0),nIcon(0){}
};

struct  GuildWarRoleRank
{
	UINT64 nRoleID;
	UINT64 nGuildID;
	std::string szName;
	std::string szGuildName;
	UINT32 nResValue;
	UINT32 nParam;
	GuildWarRoleRank():nRoleID(0),nGuildID(0),szName(""),szGuildName(""),nResValue(0),nParam(0){}
};


struct GuildWarRoleRankCmp
{
	bool operator()(const GuildWarRoleRank &p1, const GuildWarRoleRank &p2)
	{
		return p1.nResValue > p2.nResValue;
	}
};

struct FightTeamCmp
{
	bool operator()(const GuildWarRoleRank &p1, const GuildWarRoleRank &p2)
	{
		return p1.nResValue > p2.nResValue;
	}
};	

struct GuildWarRankCmp
{
	bool operator()(const GuildWarRank &p1, const GuildWarRank &p2)
	{
		if (p1.nResValue!=p2.nResValue)
			return p1.nResValue > p2.nResValue;
		else
			return p1.nPrestige > p2.nPrestige;
	}
};		


class ResWarMgr: public ITimer
{

	ResWarMgr();
	~ResWarMgr();
	DECLARE_SINGLETON(ResWarMgr);
public:
	bool Init();
	void Uninit();
	void SetState(bool bState);
	bool GetState();
	UINT32 GetStep();
	UINT32 GetResWarPVDNEID();
	UINT64 GetTeamFight(UINT32 nTeamID);
	UINT64 GetGuildFight(UINT64 nGuildID);
	void GetGuildGroup(UINT64 nGuildID,std::set<GuildFightTeam *> &setGuildTeam);
	GroupGuildInfo* GetGuildGroup(UINT32 nGroupID);
	UINT64 GetGuildID(Team *pTeam);
	UINT32 GetGuildLv(Team *pTeam);
	GuildTeam* GetGuildTeam(UINT64 nGuildID);
	void AddRoleNotice(GuildTeam *pGuildTeam, UINT64 nGuildID,UINT32 nRes);
	void AddNotice(GuildTeam *pGuildTeam,UINT64 nTargetID,UINT32 nBuffID,UINT32 nRes);
	bool UseBuffItem(UINT64 nGuildID,UINT32 nItem);
	void SyncGuildRank(GuildTeam *pFightTeam);
	void SyncGuildBuff(GuildTeam *pFightTeam);
	void SetGuildResData(KKSG::ResWarBase &data);
	void SyncGuildMineValue(GroupGuildInfo *pGroup, UINT64 nGuildID, UINT32 nRes);
	void SyncGuildMine(GuildTeam *pGuildTeam);
	void GetResWarGuildBrief(GuildTeam *pGuildTeam,GuildFightTeam *pFightTeam,KKSG::ResWarGuildBrief *pBrief);
	void GetResWarGuildRank(GuildTeam *pGuildTeam,KKSG::ResWarGuildBrief *pBrief);
	void GetResWarGuildBuff(GuildTeam *pGuildTeam,KKSG::ResWarGuildBrief *pBrief);
	void OnLeaveGuild(UINT64 nGuildID, UINT64 nRoleID);
	void OnJoinGuild(UINT64 nGuildID, CRole *pRole);
	void GetNoticeInfo(GuildTeam *pGuildTeam,KKSG::GuildBuffSimpleItem &item);
	void GetNoticeInfo(GuildTeam *pGuildTeam,KKSG::ResWarGuildBrief *pBrief);
	KKSG::ErrorCode Check(Team *pTeam);
	KKSG::ErrorCode Explore(Team *pTeam);
	void BuildMineralBattle(GuildFightTeam *pFightTeam);
	bool CheckGroup(GuildFightTeam *pSelfTeam,GuildFightTeam *pTargetTeam);
	KKSG::ErrorCode CancelExplore(Team *pTeam);
	void QueryFightRank(UINT32 nID,KKSG::ResWarRoleRankRes &res);
	void SortFightRoleLess(UINT32 nID);
	void SortGuildWarRankLess(UINT32 nID);
	void SetFightTime();
	void SendEnemyAttack(GroupGuildFightTeam *pGroupGuildFightTeam);
	void Match();
	void SetIconState(CRole *pRole);
	GuildFightTeam* AddTeam(Team *pTeam);
	UINT32 IsInResWar(UINT64 nGuildID);
	UINT32 GetGuildGroupID(UINT64 nGuildID);
	GuildFightTeam* GetGuildFightTeam(UINT64 nGuildID,UINT32 nTeamID);
	void LeaveTeam(Team *pTeam,UINT64 nRole);
	void GetPVPGroupFightTeam(UINT32 nUniID,GuildFightTeam *pSelf, GuildFightTeam *pTarget);
	GroupGuildFightTeam* GetPVPGroupFightTeam(UINT32 nSceneID);
	GuildFightTeam* GetPVEGroupFightTeam(UINT32 nUniID);
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
	void Destory();
	void SortGuildWarRoleRank(GuildTeam *pGuildTeam, GuildFightTeam *pGuildFightTeam);
	void AddResWarTeamBaseInfo(GuildFightTeam *pFightTeam, KKSG::ResWarSceneInfo *pSceneInfo,bool bSelf);
	bool CheckFightState(Team *pTeam, UINT64 nRole);
	UINT32 GetTeamOnline(GuildFightTeam *pFightTeam);
	bool CheckTeam(Team *pTeam);
	KKSG::ErrorCode CheckJoinTeam(Team *pTeam,UINT64 nRole);
	void JoinTeam(Team *pTeam,UINT64 nRole);
	GuildFightRole *GetFightRole(GuildTeam *pGuildTeam, UINT64 nRoleID);
	void ExploreRes();
	bool SetPVPData(GuildFightTeam *pSelf, GuildFightTeam *pTarget,UINT32 nUnnid);				      //同步给GS PVP
	void EnterGroupFightScene(GuildFightTeam *pFightTeam,UINT32 nSceneID);
	void StartBattlePVP();
	void SetFightTeamState(UINT32 nGroup);
	bool StartBattlePVE(GuildTeam *pGuildTeam,GuildFightTeam *pFightTeam,UINT32 nResID,UINT32 nBuffID);

	void GetGuildWarRank(UINT32 nGroupID,KKSG::ResWarFinalAll *pData);
	void GiveFinalAward();													          //最终奖励
	bool CreatePVPSceneBegin(GroupGuildFightTeam *pGroup);
	void CreatePVPSceneEnd(UINT32 nGroupID, UINT32 nSceneID);
	void ClearPVPGroup(UINT32 nGroupID);
	bool GetRoleJoin(UINT64 nRole);
	void AddRoleJoin(UINT64 nRole);
	void CreatePVESceneEnd(UINT32 nGroupID, UINT32 nSceneID);
	void SendAllIcon(UINT32 nIcon,UINT32 nState);
	UINT32 GetRiteTime();
	void SendGuildRoleIcon(UINT32 nType);
	void SendRoleIcon(CRole *pRole,UINT32 nSysID,UINT32 nState);
	void OnLogin(CRole *pRole);
	void SendTime(CRole *pRole);
	bool CheckInGroup(CRole *pRole,bool bCheck);
	void SendRoleNotice(CRole *pRole, UINT32 nNoticeType);
	void SendGuildBuffCD(Guild *pGuild, CRole *pRole,UINT32 nCD);
	void SendGuildRoleNotice();
	bool GiveAward(UINT64 nRoleID,Guild *pGuild,GuildMineralBattleReward::RowData *pRowData);
public:
	void SetEndTime(UINT32 nTime)
	{
		m_nEndTime = nTime;
	}
	void SetStep(UINT32 nStep)
	{
		m_nStep = nStep;
	}
	UINT32 GetDurationTime()
	{
		return m_nDurationTime;
	}

	struct stAuctInfo
	{
		UINT32 nRank;
		std::vector<UINT64> vecRoleID;
		stAuctInfo()
		{
			nRank = 0;
		}
	};
private:

	UINT32 m_nStep;
	time_t m_nTime;
	UINT32 m_nNextTime;
	UINT32 m_nEndTime;
	UINT32 m_nDurationTime;
	UINT32 m_nSyReadyTime;
	bool m_bOpen;
	bool m_bAward;
	UINT32 m_nUniID;
	UINT32 m_nResWarPVPTime;
	UINT32 m_nCountDownType;
	UINT32 m_nCountDownTime;
	bool m_bNotice;
	bool m_bFinal5Min;
	std::map<UINT32, GroupGuildInfo *> m_mapGroupGuildInfo;
	std::map<UINT32, GuildFightTeam *> m_mapExploreTeam;
	std::vector<GroupGuildFightTeam*> m_vecMatchPVPGroup;
	std::vector<GroupGuildFightTeam* > m_vecReportGroup;
	std::vector<GuildFightTeam* > m_vecMatchPVE;
	HTIMER m_handler;

public:
	std::map<UINT32, std::vector<GuildWarRoleRank>> m_mapGuildWarRoleRank;
	std::map<UINT32, std::vector<GuildWarRank >> m_mapGuildWarRank;
	std::set<UINT64> m_setRoleID;
};

#endif
