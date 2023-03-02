#ifndef __POSITIONDEBUGMGR_H__
#define __POSITIONDEBUGMGR_H__

#include "timer.h"

class Role;

struct Traceinfo 
{
	UINT64 qwRole;
	std::unordered_set<UINT64> m_traceEnemy;
	std::unordered_set<UINT64> m_traceRole;
	std::unordered_set<UINT64> m_traceDummyRole;

	void DoTrace(Role *pRole);
};

class PositionDebugMgr : public ITimer
{
	PositionDebugMgr();
	~PositionDebugMgr();
	DECLARE_SINGLETON(PositionDebugMgr)

public:

	bool Init();
	void Uninit();

	void TraceUnit(UINT64 qwRole, UINT64 qwUnit);
	void TraceRemoveUnit(UINT64 qwRole, UINT64 qwUnit);
	void TraceClear(UINT64 qwRole);
	std::string TraceList(UINT64 qwRole);

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

private:
	std::list<Traceinfo>::iterator FindTraceInfo(UINT64 qwRole);

	HTIMER m_handler;
	std::list<Traceinfo> m_Traces;
}; 

#endif // __POSITIONDEBUGMGR_H__