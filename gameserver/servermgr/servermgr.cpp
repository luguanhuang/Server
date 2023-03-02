#include "pch.h"
#include "servermgr.h"
#include "netproc.h"
#include "network.h"
#include "network/rslink.h"
#include "common/ptcg2r_broadcastg2r.h"
#include "cmdline.h"
#include "scene/ptcg2m_scenedestroyed.h"
#include "idutil.h"

static bool Test(const CmdLineArgs &args, std::string &outputMessage)
{
	PtcG2M_SceneDestroyed ntf;
	ntf.m_Data.set_sceneid(1000);
	ServerMgr::Instance()->SendTo(50, ntf);

	return true;
}

static bool Test2(const CmdLineArgs &args, std::string &outputMessage)
{
	PtcG2M_SceneDestroyed ntf;
	ntf.m_Data.set_sceneid(1000);
	ServerMgr::Instance()->SendToAll(ntf);

	return true;
}

bool ServerMgr::Init()
{
	CmdLine::RegistCmd("rs1", Test);
	CmdLine::RegistCmd("rs2", Test2);
	return true;
}

void ServerMgr::Uninit()
{

}

bool ServerMgr::SendTo(UINT32 serverId, const CProtocol& ptc)
{
	UINT32 realServerId = GetRealServerId(serverId);
	ptc.m_sessionID = MakeDummySession(realServerId);
	UINT32 connId = GetConn(realServerId);	
	return RSLink::Instance()->Send(connId, ptc);
}

bool ServerMgr::SendTo(UINT32 serverId, CRpc& rpc)
{
	UINT32 realServerId = GetRealServerId(serverId);
	rpc.m_sessionID = MakeDummySession(realServerId);
	UINT32 connId = GetConn(realServerId);
	return RSLink::Instance()->Send(connId, rpc);
}

bool ServerMgr::SendByRoleId(UINT64 roleId, const CProtocol& ptc)
{
	UINT32 serverId = GetServerIDByRoleID(roleId);
	return SendTo(serverId, ptc);
}

bool ServerMgr::SendByRoleId(UINT64 roleId, CRpc& rpc)
{
	UINT32 serverId = GetServerIDByRoleID(roleId);
	return SendTo(serverId, rpc);
}

bool ServerMgr::SendToGate(UINT32 serverId, UINT32 gateLine, const CProtocol& ptc)
{
	UINT32 realServerId = GetRealServerId(serverId);
	ptc.m_sessionID = MakeDummySession(realServerId, gateLine);
	UINT32 connId = GetConn(realServerId);	
	return RSLink::Instance()->Send(connId, ptc);
}

void ServerMgr::SendToAll(const CProtocol& ptc, KKSG::BroadCastG2RType type)
{
	MemoryStream *stream = MemoryStream::Get();
	const int HeadSize = sizeof(ProtocolHead);
	stream->Seek(sizeof(ProtocolHead));

	int len = ptc.Serialize(stream->GetWritePosition(), stream->GetSpace());
	if (len < 0)
	{
		LogError("serialize protocol [%s] failed.",  CProtocol::GetProtoName(ptc.m_dwType));
		return;
	}

	stream->Advance(len);

	ProtocolHead head;
	head.m_dwLen = stream->GetLength() - sizeof(UINT32);
	head.m_dwSessionID = ptc.m_sessionID;
	head.m_dwType = ptc.m_dwType;
	head.m_dwFlag = PTC_TYPE_FLAG;

	stream->Seek(0);
	stream->Write((char *)&head, sizeof(head));

	PtcG2R_BroadCastG2R ntf;
	ntf.m_Data.set_data(stream->GetBuffer(), stream->GetLength());
	ntf.m_Data.set_type(type);
	RSLink::Instance()->SendToAllLine(ntf);
}

void ServerMgr::AddServer(UINT32 connId, const KKSG::ServerIdInfo& arg)
{
	m_server2Conn[arg.serverid()] = connId;
	for (int i = 0; i < arg.serverids_size(); ++i)
	{
		m_server2server[arg.serverids(i)] = arg.serverid();
	}
}

void ServerMgr::RemoveServer(UINT32 serverId)
{
	for (auto it = m_server2server.begin(); it != m_server2server.end(); )
	{
		if (it->second == serverId)
		{
			m_server2server.erase(it++);
		}
		else
		{
			it++;
		}
	}
	m_server2Conn.erase(serverId);
}

UINT32 ServerMgr::GetRealServerId(UINT32 serverId)
{
	auto it = m_server2server.find(serverId);
	if (it == m_server2server.end())
	{
		return serverId;
	}
	return it->second;
}

UINT32 ServerMgr::GetConn(UINT32 serverId)
{
	auto it = m_server2Conn.find(serverId);
	if (it == m_server2Conn.end())
	{
		return INVALID_CONN_ID;
	}
	return it->second;
}

UINT32 ServerMgr::GetFromServerIdByRoleId(UINT64 roleId)
{
	UINT32 serverId = GetServerIDByRoleID(roleId);
	return GetRealServerId(serverId);
}

bool ServerMgr::CheckRoleIdsIsSameServerId(const std::vector<UINT64>& vlist)
{
	if (vlist.empty())
	{
		return false;
	}
	UINT32 serverid = GetFromServerIdByRoleId(vlist[0]);
	for(size_t i = 1; i < vlist.size(); ++i)
	{
		if (serverid != ServerMgr::Instance()->GetFromServerIdByRoleId(vlist[i]))
		{
			return false;
		}
	}
	return true;
}
