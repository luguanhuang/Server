#include "pch.h"
#include "serverinfomgr.h"
#include "util.h"
#include "config.h"

static bool ZSPrint(const CmdLineArgs &args, std::string &outputMessage)
{
	int size = (int)args.size();
	if(3 == size && "sn" == args[1])
	{
		UINT32 serverID = convert<UINT32>(args[2]);
		SSDebug << " servername = " << ZServerInfoMgr::Instance()->GetServerName(serverID) << END;
	}
	else if(2 == size && "cn" == args[1])
	{
		SSDebug << " servername = " << ZServerInfoMgr::Instance()->GetCurServerName() << END;
	}
	else
	{
		ZServerInfoMgr::Instance()->TestPrint();
	}
	return true;
}

INSTANCE_SINGLETON(ZServerInfoMgr)

ZServerInfoMgr::ZServerInfoMgr()
{

}

ZServerInfoMgr::~ZServerInfoMgr()
{

}

bool ZServerInfoMgr::Init()
{
	CmdLine::RegistCmd("zsinfo", ZSPrint);	

	return true;
}

void ZServerInfoMgr::Uninit()
{

}

void ZServerInfoMgr::AddServerInfo(const KKSG::ServerInfo& data)
{
	m_serverInfo[data.id()] = data;

	if (data.id() == MSConfig::Instance()->GetServerID())
	{
		MSConfig::Instance()->SetName(data.name());
		MSConfig::Instance()->SetZoneName(data.zonename());
	}

	Print(data);
}

const KKSG::ServerInfo* ZServerInfoMgr::GetServerInfo(UINT32 serverID)
{
	auto it = m_serverInfo.find(serverID);
	if(it !=  m_serverInfo.end())
	{
		return &(it->second);
	}
	return NULL;
}

void ZServerInfoMgr::TestPrint()
{
	for(auto it = m_serverInfo.begin(); it != m_serverInfo.end(); ++it)
	{
		Print(it->second);
	}
}

void ZServerInfoMgr::Print(const KKSG::ServerInfo& data)
{
	SSInfo << " serverid = " << data.id() << " servername = " << data.name() << END;
}

std::string ZServerInfoMgr::GetServerName(UINT32 serverID)
{
	const KKSG::ServerInfo* info = GetServerInfo(serverID);
	if(info)
	{
		return info->name();
	}
	return "";
}

std::string ZServerInfoMgr::GetZoneName(UINT32 serverID)
{
	const KKSG::ServerInfo* info = GetServerInfo(serverID);
	if(info)
	{
		return info->zonename();
	}
	return "";
}

std::string ZServerInfoMgr::GetCurServerName()
{
	return GetServerName(MSConfig::Instance()->GetServerID());
}
