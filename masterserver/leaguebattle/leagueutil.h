#ifndef _LEAGUE_UTIL_H_
#define _LEAGUE_UTIL_H_

class LeagueSeasonMgr;

class CProtocol;
// ���ͺ�icon������
class LeagueNoticeHelper
{
public:
	void SetTimers(LeagueSeasonMgr* mgr);
	void ClearTimers();

	// ����
	bool NoticeApply();				// �����ڼ�notice
	bool NoticeApplyEnd();			// ��������notice

	// ������
	bool NoticeRace();				// ����������
	bool NoticeRace30min();
	bool NoticeRace10min();
	bool NoticeRace3min();
	bool NoticeRace2min();
	bool NoticeRace1min();
	bool NoticeRaceStart();
	bool NoticeRaceEnd();
	bool NoticePreEliMatch();	// Ԥ����̭��

	// ��̭��
	bool NoticeEli();
	// ��һ����̭������ʱ
	bool NoticeFirstEli30min();
	bool NoticeFirstEli10min();
	bool NoticeFirstEli3min();
	bool NoticeFirstEli2min();
	bool NoticeFirstEli1min();
	bool NoticeFirstEliStart();

	// �ڶ�����̭������ʱ
	bool NoticeSecondEli3min();
	bool NoticeSecondEli2min();
	bool NoticeSecondEli1min();
	bool NoticeSecondEliStart();

	// ��������̭������ʱ
	bool NoticeThirdEli3min();
	bool NoticeThirdEli2min();
	bool NoticeThirdEli1min();
	bool NoticeThirdEliStart();
	bool NoticeEliEnd();

	// ��̭��Icon
	bool NotifyEliStartIcon();
	bool NotifyEliEndIcon();


	// ���������
	bool NoticeCrossRace();				// ����������
	bool NoticeCrossRace30min();
	bool NoticeCrossRace10min();
	bool NoticeCrossRace3min();
	bool NoticeCrossRace2min();
	bool NoticeCrossRace1min();
	bool NoticeCrossRaceStart();
	bool NoticeCrossRaceEnd();
	bool NoticeCrossPreEliMatch();	// Ԥ����̭��

	// �����̭��
	bool NoticeCrossEli();
	// ��һ�ֿ����̭������ʱ
	bool NoticeFirstCrossEli30min();
	bool NoticeFirstCrossEli10min();
	bool NoticeFirstCrossEli3min();
	bool NoticeFirstCrossEli2min();
	bool NoticeFirstCrossEli1min();
	bool NoticeFirstCrossEliStart();

	// �ڶ��ֿ����̭������ʱ
	bool NoticeSecondCrossEli3min();
	bool NoticeSecondCrossEli2min();
	bool NoticeSecondCrossEli1min();
	bool NoticeSecondCrossEliStart();

	// �����ֿ����̭������ʱ
	bool NoticeThirdCrossEli3min();
	bool NoticeThirdCrossEli2min();
	bool NoticeThirdCrossEli1min();
	bool NoticeThirdCrossEliStart();
	bool NoticeCrossEliEnd();

	// �����̭��Icon
	bool NotifyCrossEliStartIcon();
	bool NotifyCrossEliEndIcon();


	bool NotifyRacePointBeginIcon();
	bool NotiyfRacePointEndIcon();
	bool NotifyCrossRacePointBeginIcon();
	bool NotiyfCrossRacePointEndIcon();

	void NotifyToAll(const CProtocol& ntf);
	void NotifyToLTeamMember(const CProtocol& ntf);
	void NotifyToEliMember(const CProtocol& ntf);
	void NotifyToCrossMember(const CProtocol& ntf);


private:
	std::list<UINT32> m_timers;
};

#endif