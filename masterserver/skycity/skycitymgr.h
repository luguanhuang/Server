#ifndef __SKYCITYMGR_H
#define __SKYCITYMGR_H
#include "timer.h"
#include "define/skycitydef.h"
class CRole;
class Team;
class CSceneSwitch;


enum SkyCityFlow
{
	eSkyCityFlowIdle			= 0,			//空闲状态
	eSkyCityFlowCreateWaitRoom	= 1,			//创建等待场景
	eSkyCityFlowWaitRoom		= 2,			//创建等待房间
	eSkyCityFlowSigUp			= 3,			//报名
	eSkyCityFlowFirstMatch		= 4,			//第一次匹配匹配阶段
	eSkyCityFlowMatch			= 5,			//第一次匹配匹配阶段
	eSkyCityFlowChangeScene		= 6,			//改变场景
	eSkyCityFlowEndFight		= 7,			//结束战斗
	eSkyCityFlowAward			= 8,			//派奖
	eSkyCityFlowEnd				= 9,			//结束流程
	eSkyCityIconDisapper        = 10,			//结束图标消失
};



enum SkyCityGame
{
	eSkyCityGameWin		=1,
	eSkyCityGameFail	=2,
	eSkyCityGameFlat	=3,
	eSkyCityExit		=4,	
};

enum SkyCityLoginState
{
	eSkyCityLoginIn			   = 1,
	eSkyCityLoginOut		   = 2,
};

#define  SKYCITY_WAIT_SCENE				18
#define  SKYCITY_FIGHT_SCENE			19
#define  SKYCITY_TEAM_MAX_MEMBERS		3
#define  SKYCITY_DNE_ID					4002
#define  SKYCITY_GOOD_TEAM				32
#define  SKYCITY_FLOOR_MAXTEAMS			32
#define  SKYCITY_TIME_SPAN				50


using namespace SkyCityDef;

class SkyCityMgr : public ITimer
{
	SkyCityMgr();
	~SkyCityMgr();
	DECLARE_SINGLETON(SkyCityMgr);
public:
	struct SinlePPTCmp
	{
		bool operator()(const SinglePPT &p1, const SinglePPT &p2)
		{
			return p1.nPPT > p2.nPPT;
		}
	};
	struct TeamInfoCmp
	{
		bool operator()(const FightTeam* p1, const FightTeam *p2)
		{
			return p1->nPPT > p2->nPPT;
		}
	};
	
private:
	//////////////////////////////////报名信息////////////////////////////////
	std::map<UINT32, FightTeam*> m_mapFightTeam;			//战队信息 KEY===TEAMID
	std::map<UINT64, UINT32> m_mapSingle;				//单人
	std::map<UINT32, std::vector<FightTeam*>> m_mapSortFightTeam;
	std::map<UINT32, std::set<GroupTeam * > > m_mapGroup;	
	std::deque<CRole *> m_deqRole;
	std::deque<GroupTeam *> m_deqGroupTeam;
public:
	bool Init();		
	void Uninit();
	void Match();						//匹配分组
	bool CheckLevel(CRole *pRole);
	UINT32 GetEnterLv();
	void ClearPPT();
	void FirstMatch();
	bool CheckTeam();
	void KickWait();
	void AddMemberTeam();
	void EnterToHall();
	void SingleToTeam();
	void TeamEnterHall(Team *pTeam);
	void ClearZombieTeam();
	void BuildTeamData();
	void SetTeamState();
	void FirstFillFloor();
	void FillFloor();
	void OnRoleLogout(CRole *pRole);
	void OnLeaveTeam(CRole *pRole,bool bDel=false);
	void SetFightRoleState(CRole *pRole);
	void OnClearFightRole(CRole *pRole);
	void OnWaitSceneLogout(CRole *pRole);
	void ClearGroup();
	void BuildGroupTeam();
	bool CreateBattle();
	void HighAwardNotice(FightTeam *pFightTeam);
	void EnterFightScene();
	void GiveFightTeam(FightTeam *pFightTeam);
	void GiveAward();
	bool AddTeam(Team *pTeam);
	void GetNoticeRole(std::vector<UINT64> &vecRole);
	void ActivityNotice();
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
	void FillRoleSCRoleStatistics(UINT32 nGroupID,UINT64 nRole,UINT32 nKiller, UINT32 nDeadCount, double nDamage);
	void EnterGroupFightScene(FightTeam *pFightTeam,UINT32 nSceneID);
	void CreateFightSceneEnd(UINT32 nGroupID, UINT32 nFloor, UINT32 nSceneID);
	void OnLeaveFightScene(CRole *pRole,UINT32 nFromSceneID,UINT32 nToSceneID);
	void OnLeaveWaitRoom(CRole *pRole,UINT32 nSceneID);
	bool EnterWaitRoom(CRole *pRole,bool bSingle=true);
	void CreateSceneWaitEnd(UINT32 nWarType, UINT32 nFloor, UINT32 nSceneID);
	bool CreateSceneWaitBegin();
	void AddSkyCityStatistics(FightTeam *pFightTeam, KKSG::SkyCitySceneInfo *pInfo);
	void AddSkyCityTeamBaseInfo(FightTeam *pFightTeam,KKSG::SkyCitySceneInfo *pInfo,bool bSelf);
	bool CreateSceneBegin(GroupTeam *pGroup);
	void SortFightTeamLess();
	void SortSinglePPT();
	bool GetStartBattle(Team *pTeam);
	void SetStep(UINT32 nStep){m_nStep = nStep;}
	void SetState(bool bState) {m_bOpen = bState;};
	void SetGames(UINT32 nGames){m_nGames = nGames;}
	void SetNextTime(){m_nNextTime = 0;}
	void ClearFightTeam(FightTeam *pFightTeam);
	void DesoryGame();
	void AddGameResult(UINT32 nGroupID,UINT32 nTeamID);
	void SetRoleFloor(FightTeam *pFightTeam,bool bOnline ,UINT32 nFloor);
	GroupTeam * GetGroupTeam(UINT32 nGroupID);
	void NotifyEndIcon(UINT64 nRoleID,KKSG::HallIconState eState);
	void OnLogin(CRole *pRole);
	void ClearIcon();
	void ClearRoleIcon(CRole *pRole);
private:
	UINT32 m_nIconTime;
	UINT32 m_nStep;
	UINT64 m_dwMillTime;
	UINT64 m_nNextTime;
	UINT32 m_nGames;
	bool  m_bOpen;
	bool  m_bAward;
	HTIMER m_handler;
	UINT32 m_nSkyCityWaitID;
	UINT32 m_nGroupID;
	UINT32 m_nSkyCityWaitingTID;
	UINT32 m_nSkyCityFirstHalfTime;
	UINT32 m_nSkyCityHalfTime;
	UINT32 m_nSkyCitySpaceTime;
	UINT32 m_nSkyCitySecondHalfTime;
	UINT32 m_nSkyCityFloor;
	UINT32 m_nSkyCityGames;	
	UINT32 m_nSkyCitySecFloorTeam;
	UINT32 m_nSkyCityWaitingRoomTime;
	UINT32 m_nSkyCityWholeTime;
	UINT32 m_nSkyCityChangeSceneTime;
	UINT32 m_nSkyCityEnterLv;
	bool   m_bOpen1MIN;
	bool   m_bOpen2MIN;
	bool   m_bOpen3MIN;

	std::set<UINT64> m_nRoleID;
};

#endif