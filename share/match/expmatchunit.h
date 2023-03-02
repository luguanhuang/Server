#ifndef __EXPMATCHUNIT_H__
#define __EXPMATCHUNIT_H__

#include "matchbase.h"

class MatchUnitExp : public MatchUnit
{
public:
	MatchUnitExp(KMatchID matchID, int type, const KKSG::KMatchUnit& kunit)
		:MatchUnit(matchID, type, kunit)
	{
		ClearWait();
	}

	void SetWait(int roomID, UINT32 now);
	void ClearWait();
	inline int GetRoomID() { return m_waitRoomID; }
	inline UINT32 GetWaitTime() { return m_time; }
	inline bool IsInWait() { return 0 != m_waitRoomID; }
	bool IsWaitTimeEnd(UINT32 now);

	int GetExpID();
	UINT32 GetMatchCount();
	std::string GetRoleName();
	const KKSG::KMatchExp* GetExpData();

private:
	int m_waitRoomID;//�ȴ�ȷ�ϱ�ʶ
	UINT32 m_time;//��ʼ�ȴ���ʱ��
};

#endif