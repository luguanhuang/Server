#include "pch.h"
#include "gateprofile.h"
#include "config.h"
#include "network/clientlink.h"

INSTANCE_SINGLETON(CGateProfile)


CGateProfile::CGateProfile()
:m_broadCastNum(0)
,m_broadCastClientNum(0)
{

}

CGateProfile::~CGateProfile()
{
}

bool CGateProfile::Init()
{
	StartTimer();

	std::stringstream ss;
	ss << "gateserver_";
	ss << GateConfig::Instance()->GetLine();
	SetFileName(ss.str());
	return true;
}

void CGateProfile::Uninit()
{
	StopTimer();
}

void CGateProfile::DoProfile(FILE* fp)
{
	fprintf(fp, "BroadCastNum: %llu\n", m_broadCastNum);
	fprintf(fp, "BroadCastClientNum: %llu\n", m_broadCastClientNum);
	m_broadCastNum = 0;
	m_broadCastClientNum = 0;

	fprintf(fp, "RecvPtcNum: %llu\n", CClientLink::Instance()->GetRecvPtcNum());
	fprintf(fp, "RecvRpcNum: %llu\n", CClientLink::Instance()->GetRecvRpcNum());
	fprintf(fp, "SendPtcNum: %llu\n", CClientLink::Instance()->GetSendPtcNum());
	fprintf(fp, "SendRpcNum: %llu\n", CClientLink::Instance()->GetSendRpcNum());
	CClientLink::Instance()->ClearStatNum();
}
