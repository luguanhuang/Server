#include "pch.h"
#include "connectionmgr.h"
#include "timeutil.h"
#include "network/clientlink.h"

INSTANCE_SINGLETON(ConnectionMgr)

ConnectionMgr::ConnectionMgr()
{
}

ConnectionMgr::~ConnectionMgr()
{

}

bool ConnectionMgr::Init()
{
	m_timeList.SetTimeOut(10);
	m_timeList.SetCallBack(std::bind(&ConnectionMgr::OnTimeOut, this, std::placeholders::_1));

	CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);	
	return true;
}

void ConnectionMgr::Uninit()
{

}

void ConnectionMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	UINT32 now = TimeUtil::GetTime();
	m_timeList.Update(now);
}

void ConnectionMgr::OnTimeOut(const UINT32& connId)
{
	LogWarn("client time out close %u", connId);
	CClientLink::Instance()->Close(connId);
}

void ConnectionMgr::AddConn(UINT32 connId)
{
	UINT32 now = TimeUtil::GetTime();
	m_timeList.Add(connId, now);
}

void ConnectionMgr::RemoveConn(UINT32 connId)
{
	m_timeList.Remove(connId);
}
