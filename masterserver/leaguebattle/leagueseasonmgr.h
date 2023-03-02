#ifndef _LEAGUE_BATTLE_SEASON_MGR_H_
#define _LEAGUE_BATTLE_SEASON_MGR_H_

#include "util/utilsingleton.h"
#include "timer.h"
#include "leagueutil.h"
#include "leaguebattle/leagueseasonbase.h"

class CRole;
class LeagueSeasonMgr : public LeagueSeasonBase, public ITimer, public Singleton<LeagueSeasonMgr>
{
public:
	friend class LeagueNoticeHelper;
	LeagueSeasonMgr();
	virtual ~LeagueSeasonMgr();

	bool Init();
	void Uninit();

	void LoadSeasonInfoFromDB();
	void SaveSeasonInfoToDB();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
	void OnStateChange(KKSG::LeagueBattleTimeState oldState, KKSG::LeagueBattleTimeState curState);
	// ��������, ������������
	void OnSeasonEnd();
	void OnSeasonOpen();

	UINT32 GetMatchLeftTime();
	UINT32 GetRankRewardLeftTime();
	UINT32 GetCrossRankRewardLeftTime();
	UINT32 GetSeasonNum() { return m_seasonNum; }
	bool IsSeasonOpen() { return m_isSeasonOpen; }
	bool IsInApplyTime();
	bool IsInPointRaceMatchTime();

	void OnUnlockLevelSeal(UINT32 type);

	void SetSeasonOpen(bool open = true);

	void OnRoleLogin(CRole* pRole);

	void SetRaceMatchNoLimit(bool is = true) { m_isRaceMatchNoLimit = is; }


	void ReportTop2World();	// �ϱ�top8��world
	void OnReportResult(bool success);

private:
	void CheckSeasonOpen();

	KKSG::LeagueBattleTimeState GetCurState(UINT32 now);

	void SetUpTimers();
	void ClearTimers();

	void NotifyTimeState();

	// ��̭��ÿ�ֶ�ʱ��
	bool EleminationRoundTimer();

private:
	bool m_dirty;
	bool m_isSeasonOpen;
	bool m_report2World;
	UINT32 m_lastUpdateTime;
	UINT32 m_seasonNum;
	UINT32 m_lastSeasonOpenTime;

	bool m_reporting;		// �����ϱ���ǿ��world������

	std::list<UINT32> m_timers;

	// GM����
	bool m_isRaceMatchNoLimit;		// ƥ�����Ƿ�û��ʱ������

	LeagueNoticeHelper m_noticeHelper;
};

#endif