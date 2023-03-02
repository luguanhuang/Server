#include "pch.h"
#include "gateinfo.h"
#include "util.h"
#include "pb/enum.pb.h"
#include "config.h"
#include "timeutil.h"
#include "verify/tokenverifymgr.h"

void GateInfo::ParseChannel(const std::string& str)
{
	std::vector<std::string> vec = Split(str, '|');	
	for (size_t i = 0; i < vec.size(); ++i)
	{
		UINT32 oneChannel = convert<UINT32>(vec[i]);
		channel.push_back(oneChannel);
	}
	// 为了测试方便，所有的分区对开发帐号(LOGIN_PASSWORD)都可见
	if (std::find(channel.begin(), channel.end(), KKSG::LOGIN_PASSWORD) == channel.end())
	{
		channel.push_back(KKSG::LOGIN_PASSWORD);
	}
}

void GateInfo::ParseIpAddr(const std::string& str, std::vector<GateIPAddr>& hosts)
{
	hosts.clear();
	std::vector<std::string> vec = Split(str, '|');	
	for (UINT32 j = 0; j < vec.size(); ++j)
	{
		UINT32 st = vec[j].find(':');
		if (st != std::string::npos)
		{
			GateIPAddr addr;
			addr.ip = vec[j].substr(0, st);
			addr.port = convert<int>(vec[j].substr(st+1));
			hosts.push_back(addr);
		}
		else
		{
			LogWarn("server id [%d:%s] ip [%s] not found port", serverID, serverName.c_str(), vec[j].c_str());
		}
	}
}

void GateInfo::UpdateState()
{
	if (setState >= ServerSetState_Max)
	{
		setState = ServerSetState_Auto;
	}

	state = setState;
	// 自动状态
	if (setState == ServerSetState_Auto)
	{
		// 在线人数爆满
		if (onlineRole >= LoginConfig::Instance()->GetOnlineFullNum())
		{
			state = ServerSetState_Full;
		}
		// 注册人数流畅
		else if (registerAccount < LoginConfig::Instance()->GetRegisterSmoothNum())
		{
			state = ServerSetState_Smooth;
		}
		// 注册人数拥挤
		else if (registerAccount < LoginConfig::Instance()->GetRegisterFullNum())
		{
			state = ServerSetState_Hot;
		}
		// 注册人数爆满
		else
		{
			state = ServerSetState_Full;
		}
	}

	UpdateClientState();
}

void GateInfo::UpdateClientState()
{
	switch (state)
	{
		case ServerSetState_Maintain:
		{
			clientState = KKSG::ServerState_Maintain;
			clientFlag = KKSG::ServerFlag_Maintain;
			break;
		}
		case ServerSetState_New:
		{
			clientState = KKSG::ServerState_Smooth;
			clientFlag = KKSG::ServerFlag_New;
			break;
		}
		case ServerSetState_Smooth:
		{
			clientState = KKSG::ServerState_Smooth;
			clientFlag = KKSG::ServerFlag_Smooth;
			break;
		}
		case ServerSetState_Hot:
		{
			clientState = KKSG::ServerState_Hot;
			clientFlag = KKSG::ServerFlag_Hot;
			break;
		}
		case ServerSetState_Full:
		{
			clientState = KKSG::ServerState_Full;
			clientFlag = KKSG::ServerFlag_Full;
			break;
		}
		case ServerSetState_Recommend:
		{
			clientState = KKSG::ServerState_Smooth;
			clientFlag = KKSG::ServerFlag_Recommend;
			break;
		}
		default:
		{
			LogError("invalid server state[%d]", state);
			clientState = KKSG::ServerState_Smooth;
			clientFlag = KKSG::ServerFlag_Smooth;
		}
	}

	// 自动推荐
	if (autoRecommend)
	{
		clientFlag = KKSG::ServerFlag_Recommend;
	}
}

GateIPAddr* GateInfo::GetRandomHost(int ccType, bool isFreeFlow)
{
	GateIPAddr* pAddr = NULL;
	std::vector<GateIPAddr>* pVec = NULL;
	if (isFreeFlow)
	{
		pVec = &FreeHosts;
		/*
		if (ccType == CCType_Normal)
		{
			pVec = &Hosts;
		}
		else if (ccType == CCType_LianTong)
		{
			pVec = &LianTongHosts;
		}
		else if (ccType == CCType_DianXin)
		{
			pVec = &DianXinHosts;
		}
		else if (ccType == CCType_YiDong)
		{
			pVec = &YiDongHosts;
		}
		*/
	}
	else
	{
		pVec = &Hosts;
	}

	if (pVec == NULL)
	{
		return NULL;
	}

	if (pVec->empty())
	{
		// 如果免流ip没有配置，返回正常的ip
		if (pVec != &Hosts)
		{
			pVec = &Hosts;
		}
		if (pVec->empty())
		{
			return NULL;
		}
	}
	return &(*pVec)[rand()%pVec->size()];
}

void GateInfo::FillGateInfo(int ccType, bool isFreeFlow, KKSG::LoginGateData& data)
{
	data.set_serverid(serverID);
	data.set_state(clientState);
	data.set_flag(clientFlag);
	data.set_zonename(zoneName);
	data.set_servername(serverName);
	data.set_isbackflow(IsBackFlowServer());
	data.set_backflowlevel(backFlowLevel);

	GateIPAddr* addr = GetRandomHost(ccType, isFreeFlow);
	if (addr)
	{
		data.set_ip(addr->ip);
		data.set_port(addr->port);
	}
}
