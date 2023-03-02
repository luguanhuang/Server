#include "pch.h"
#include "gmlink.h"
#include "config.h"
#include "common/ptci2p_registerloginntf.h"
#include "gatemgr/gatemgr.h"

INSTANCE_SINGLETON(GMLink)

GMLink::GMLink()
{
	m_connid = -1;
}

GMLink::~GMLink()
{

}

void GMLink::OnConnectSucceed(UINT32 dwConnID)
{
	m_connid = dwConnID;

	PtcI2P_RegisterLoginNtf ntf;
	ntf.m_Data.set_platform(LoginConfig::Instance()->GetPlatform());
	ntf.m_Data.set_serverid(LoginConfig::Instance()->GetServerID());
	GateMgr::Instance()->ServerInfoNotifyToIdip(ntf.m_Data.mutable_data());
	Send(ntf);

	SSInfo<<"connect to idip server ok! connid:"<<dwConnID<<END;
}

void GMLink::OnConnectFailed()
{
	SSInfo<<"connect to idip server failed!"<<END;
}

void GMLink::OnPassiveConnect(UINT32 dwConnID)
{
	SSError<<__FUNCTION__<<END;
	assert(false);
}

void GMLink::OnClose(UINT32 dwConnID, int err)
{
	SSError<<"idip server disconnected, connid:"<<dwConnID<<END;
}

const std::string& GMLink::GetName()
{
	static const std::string name = "idiplink";
	return name;
}

bool GMLink::Send(const CProtocol& ptc)
{
	return BaseLink::Send(m_connid, ptc);
}

bool GMLink::Send(CRpc& rpc)
{
	return BaseLink::Send(m_connid, rpc);
}
