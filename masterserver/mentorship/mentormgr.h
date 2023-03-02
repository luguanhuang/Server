#ifndef __MENTORMGR_H__
#define __MENTORMGR_H__

#include "timer.h"

class MentorShipOneSide;
#define MENTORAUTOBREAKTIME 86400

struct BreakApplyPair
{
	BreakApplyPair()
	{
		m_srcID = 0;
		m_destID = 0;
		//m_time = 0;
	}

	bool operator < (const BreakApplyPair& pair) const
	{
		if (m_srcID == pair.m_srcID && m_destID == pair.m_destID)
		{
			return false;
		}
		if (m_srcID < pair.m_srcID)
		{
			return true;
		}
		else if (m_srcID == pair.m_srcID)
		{
			if (m_destID < pair.m_destID)
			{
				return true;
			}
			else
				return false;
		}

		return false;
	}

	UINT64 m_srcID;
	UINT64 m_destID;
	//int m_time;
};

class MentorMgr : public ITimer
{
public:
	MentorMgr();
	~MentorMgr();
	DECLARE_SINGLETON(MentorMgr)

public:
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	bool Init();
	void Uninit();
	bool OnDayPass();
	void OnLogin(UINT64 roleID);

	bool LoadFromDb();
	void Save2Db(bool isAll=true);
	void QuickSave(MentorShipOneSide* pOneSide);

	void AddChangedMentor(UINT64 roleID);

	MentorShipOneSide* GetMentorShip(UINT64 roleID, bool force=true);

	void AddAutoBreak(BreakApplyPair& pair, int time);
	void RemoveAutoBreak(BreakApplyPair& pair);

	KKSG::EMentorRelationPosition GetMentorPos(UINT64 srcID, UINT64 destID);

private:
	bool _ParseFromDb(UINT64 roleID, const KKSG::MentorAllData2Db& data);

private:
	std::map<UINT64, MentorShipOneSide*> m_mentorshipList;
	std::set<UINT64> m_changedList;

	std::map<BreakApplyPair, int> m_autoBreakList;
	HTIMER m_timer;
	HTIMER m_dayTimer;
};

#endif // 
