#include "pch.h"
#include "gmlink.h"
#include "network.h"
#include "netproc.h"
#include "config.h"
#include "crpc.h"
#include "common/ptcm2p_registmsserver.h"


INSTANCE_SINGLETON(GMLink)

GMLink::GMLink()
{
}

GMLink::~GMLink()
{

}

void GMLink::OnConnectSucceed(UINT32 dwConnID)
{
	LogInfo("connect to idip server ok!");

	PtcM2P_RegistMSServer ptc;
	ptc.m_Data.set_serverid(MSConfig::Instance()->GetServerID());
	ptc.m_Data.set_servername(MSConfig::Instance()->GetServerName());
	std::set<UINT32> vServerIdList;
	vServerIdList = MSConfig::Instance()->GetServerIds();
	for (auto it = vServerIdList.begin(); it != vServerIdList.end(); it ++)
	{
		ptc.m_Data.add_serverids(*it);
	}
	Send(dwConnID, ptc);
}

void GMLink::OnConnectFailed()
{
	LogError("connect to idip server failed!");
}

void GMLink::OnPassiveConnect(UINT32 dwConnID)
{
	LogError("%s", __FUNCTION__);
	assert(false);
}

void GMLink::OnClose(UINT32 dwConnID, int err)
{
	LogError("%s", __FUNCTION__);
}


void GMLink::Close()
{
	LogError("%s", __FUNCTION__);
}

const std::string & GMLink::GetName()
{
	static const std::string LinkName = "idiplink";
	return LinkName;
}
