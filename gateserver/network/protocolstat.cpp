#include "pch.h"
#include "protocolstat.h"
#include "timeutil.h"
#include "common/rpcc2g_synctime.h"


INSTANCE_SINGLETON(CProtocolStat)


CProtocolStat::CProtocolStat()
{
}

CProtocolStat::~CProtocolStat()
{
}

bool CProtocolStat::Init()
{
	return true;
}

void CProtocolStat::Uninit()
{

}

void CProtocolStat::OnRecv(UINT64 qwSessionID, UINT32 dwType, UINT32 dwLen)
{
	if(dwType == RPCC2G_SYNCTIME_TYPE)
	{
		CStatInfo& roInfo = m_oStatMap[qwSessionID];
		roInfo.m_qwRecvTime = TimeUtil::GetMilliSecond();
	}
}

void CProtocolStat::OnReply(UINT64 qwSessionID, UINT32 dwType)
{
	if(dwType == RPCC2G_SYNCTIME_TYPE)
	{
		CStatInfo& roInfo = m_oStatMap[qwSessionID];
		roInfo.m_qwReplyTime = TimeUtil::GetMilliSecond();
		UINT64 qwTimeUsed = (roInfo.m_qwReplyTime >= roInfo.m_qwRecvTime ? roInfo.m_qwReplyTime - roInfo.m_qwRecvTime : 0);
		if(qwTimeUsed >= 50)
		{
			LogWarn("Session %llu proccess RPCC2G_SYNCTIME_TYPE use time %llu ms", qwSessionID, qwTimeUsed);
		}
	}
}