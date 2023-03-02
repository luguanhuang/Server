#ifndef _SERVER_MGR_H_
#define _SERVER_MGR_H_

#include "util/utilsingleton.h"
#include "protocol.h"
#include "crpc.h"
#include "config.h"

class ServerMgr : public Singleton<ServerMgr>
{
public:
	bool Init();
	void Uninit();

	bool SendTo(UINT32 serverId, const CProtocol& ptc);
	bool SendTo(UINT32 serverId, CRpc& rpc);
	bool SendToGate(UINT32 serverId, UINT32 gateLine, const CProtocol& ptc);
	bool SendByRoleId(UINT64 roleId, const CProtocol& ptc);
	bool SendByRoleId(UINT64 roleId, CRpc& rpc);

	void SendToAll(const CProtocol& ptc, KKSG::BroadCastG2RType type = KKSG::BroadCastG2RType_MS);

	void AddServer(UINT32 connId, const KKSG::ServerIdInfo& arg);
	void RemoveServer(UINT32 serverId);

	UINT32 GetFromServerIdByRoleId(UINT64 roleId);
	UINT32 GetRealServerId(UINT32 serverId);
	bool CheckRoleIdsIsSameServerId(const std::vector<UINT64>& vlist);

private:

	UINT32 GetConn(UINT32 serverId);

	// 伪造session
	inline UINT64 MakeDummySession(UINT32 serverId, UINT32 gateLine = 0)
	{
		UINT64 newSessionID = (gateLine & 0xFFFF);
		newSessionID <<= SessionGateIDShift;
		UINT64 temp = (serverId & 0xFFFF);
		temp <<= 32;
		UINT64 line = (GSConfig::Instance()->GetLine() & 0xFFFF) << 16;
		temp |= line;
		newSessionID |= temp;
		return newSessionID;
	}

private:
	std::unordered_map<UINT32, UINT32> m_server2Conn;		// serverId 对应 router的连接id (合服后只保留一个)
	std::unordered_map<UINT32, UINT32> m_server2server;		// 对应的实际serverId
};

#endif