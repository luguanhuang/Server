#ifndef _LEAGUE_SEASON_BASE_H_
#define _LEAGUE_SEASON_BASE_H_

// ���������������
class LeagueSeasonBase
{
public:
	LeagueSeasonBase();
	virtual ~LeagueSeasonBase();

	KKSG::LeagueBattleTimeState GetTodayState() { return m_todayState; }
	UINT32 GetMatchLeftTime();

	UINT32 GetPreStartTime();
	UINT32 GetNextStartTime();

	void DumpSeason(std::string& output);

	bool IsCrossPointRaceDay();
	bool IsInCrossState();
	bool IsShowEli();
	KKSG::LeagueEliType GetEliType();
	

protected:
	void BuildTimeState();
	KKSG::LeagueBattleTimeState GetCurStateByTime(UINT32 now);

protected:
	UINT32 m_seasonOpenTime;
	UINT32 m_seasonEndTime;
	UINT32 m_nextSeasonOpenTime;

	std::map<UINT32, KKSG::LeagueBattleTimeState> m_stateMap;	// ״̬Map
	UINT32 m_lastCanOpenTime;									// �����Կ�����ʱ��(�����ǵ����һ�����������)
	UINT32 m_lastApplyTime;										// ���һ�����������
	UINT32 m_lastRacePointDayZero;								// ���һ������������
	UINT32 m_eleMapBuildTime;									// ������ǿ����ͼ����ʱ��(ͬʱҲ�Ǳ���������������ʱ��)
	UINT32 m_selfEndTime;										// ��������ʱ��
	UINT32 m_crossStartTime;									// �����ʼʱ��
	UINT32 m_crossEleMapBuildTime;								// �����ǿ����ͼ����ʱ��(ͬʱҲ�ǿ��������������ʱ��)
	UINT32 m_lastCrossRacePointDayZero;							// ���һ���������������
//	UINT32 m_seasonClearDayZero;								// �������ʱ��
	KKSG::LeagueBattleTimeState m_todayState;					// �����״̬

	bool m_clearSelfServer;										// �����������Ƿ�����
	UINT32 m_clearSelfTime;										// ��������������ʱ��
	UINT32 m_lastCheckOpenTime;									// �ϴμ������������ʱ��

	UINT32 m_selfEliShowStartTime;								// ������̭��չʾ��ʼʱ��
	UINT32 m_selfEliShowEndTime;								// ������̭��չʾ����ʱ��
	UINT32 m_crossEliShowStartTime;								// �����̭��չʾ��ʼʱ��
	UINT32 m_crossEliShowEndTime;								// �����̭��չʾ����ʱ��
};

#endif