#ifndef __HORSERIDEMGR_H__
#define __HORSERIDEMGR_H__

#include "pvpbasedata.h"
#include "table/HorseRace.h"
#include "table/Horse.h"
#include "timer.h"
#include "scene/sceneswitch.h"

class Role;

class HorseRideMgr : public ITimer
{
	HorseRideMgr();
	~HorseRideMgr();
	DECLARE_SINGLETON(HorseRideMgr);
public:
	enum State
	{
		eStop = 0,
		eRuning  = 1,
		eEnd	 = 2,
	};
	enum GuildResFlow
	{
		eHorseRaceIdle					= 0,		//空闲状态
		eHorseRaceMatch	      			= 1,		//分组
		eHorseRaceEnter					= 2,		//进入比赛场景
		eHorseRaceFailNotice			= 3,		//玩家失败提示
		eHorseRaceEnd					= 4,        //流程结束
	};
	bool Init();
	void Uninit();
	void SendNotice(UINT32 nMin);
	void CheckOpenNotice(UINT32 nSystemID,UINT32 nState);
	void HallIconNoticeCheck(Role* pRole,UINT32 nSystemID,UINT32 nState);
	void Open();
	bool IsOpen();
	void SetState(bool bState);
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
	void SendFailTips();
	void Destory();
	void OnRoleLogin(Role* pRole);
	void HorseMatch();
	void ChangeScene(CSceneSwitch::RoleList &rolelist);
	bool Enter();
	HorseRace::RowData* GetRank(UINT32 nRank,UINT32 nSceneID);
	Horse::RowData* GetHorse(UINT32 nSceneID);
private:
	HTIMER m_handler;
	struct IconState
	{
		UINT32 nState;
		UINT32 nTime;
		UINT32 nSystemID;
	};
	IconState m_Icon;
	UINT32 m_nTime;
	UINT32 m_nStep;
	UINT32 m_nNoticeTime;
	bool m_bOpen;
	bool m_bEnter;
	std::list<UINT64> m_RoleIDs;
	HorseRace m_awardTable;
	std::map<UINT32, std::vector<HorseRace::RowData*> > m_mapAward;
	Horse m_horseTable;
	UINT32 m_nFailTipsTime;
};

#endif