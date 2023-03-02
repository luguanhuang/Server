#ifndef _CONNECTION_MGR_H_
#define _CONNECTION_MGR_H_

#include "util/timelist.h"
#include "timer.h"

class ConnectionMgr : public ITimer
{
	ConnectionMgr();
	~ConnectionMgr();
	DECLARE_SINGLETON(ConnectionMgr)

public:
	bool Init();
	void Uninit();

	void AddConn(UINT32 connId);
	void RemoveConn(UINT32 connId);

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
	void OnTimeOut(const UINT32& connId);
public:
	TimeList<UINT32, UINT32> m_timeList;
};

#endif